// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameplayAbility_ComboAttack.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimMontage.h"
#include "ActionGame/AGGamplayTags.h"
#include "ActionGame/AbilitySystem/Attributes/AGPlayerSet.h"
#include "ActionGame/Character/AGPlayer.h"
#include "ActionGame/Data/AGTableData.h"
#include "ActionGame/Player/AGPlayerState.h"
#include "ActionGame/System/AGAssetManager.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ActionGame/AbilitySystem/Effects/AGGameplayEffect_AttackDamage.h"
#include "ActionGame/Player/AGPlayerController.h"

UAGGameplayAbility_ComboAttack::UAGGameplayAbility_ComboAttack()
{
	// 어빌리티의 실행 중지 여부 설정
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	// AbilityTags에 원하는 태그 추가
	AbilityTags.AddTag(AGGameplayTags::Ability_ComboAttack);

	// 이미 활성화된 상태에서 또 활성화되지 않게
	BlockAbilitiesWithTag.AddTag(AGGameplayTags::Ability_ComboAttack);
}

void UAGGameplayAbility_ComboAttack::PlayAttackMontage(UAnimMontage* MontageToPlay)
{
	if (false == IsValid(MontageToPlay))
	{
		return;
	}
	
	AAGPlayer* AvatarPlayer = Cast<AAGPlayer>(GetAvatarActorFromActorInfo());
	if (false == IsValid(AvatarPlayer))
	{
		return;
	}

	UAnimInstance* AnimInstance = AvatarPlayer->GetMesh()->GetAnimInstance();
	if (false == IsValid(AnimInstance))
	{
		return;
	}

	AvatarPlayer->PlayAnimMontage(MontageToPlay);

	// 몽타주 종료 델리게이트에 OnMontageEnded함수 바인딩
	FOnMontageEnded MontageEnded;
	MontageEnded.BindUObject(this, &ThisClass::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEnded, MontageToPlay);
}

void UAGGameplayAbility_ComboAttack::ExecuteComboAttack()
{
	// 콤보 패턴 데이터 찾기
	FAttackPatternData* AttackPattern = FindAttackPattern();

	if (AttackPattern && false == IsFinish)
	{
		// 공격 애니메이션 재생
		PlayAttackMontage(AttackPattern->AttackMontage);
	
		// 콤보 상태 업데이트
		InputAttackType = EAttackType::None;
		PreviousAttackType = AttackPattern->CurrentAttackType;
		
		if (IsValid(AGPlayer))
		{
			// 타겟 탐색
			AGPlayer->SelectAttackTarget();
			
			AAGPlayerController* Controller = Cast<AAGPlayerController>(AGPlayer->GetController());
			if (IsValid(Controller))
			{
				// 타겟 방향 혹은 방향키 방향으로 회전
				// 타겟 방향이 우선 순위
				Controller->RotateCharacterToTarget();
			}
		}

		// 다음 공격이 불가능한 공격 타입일 때 마지막 공격이라는 옵션 켜줌
		if (false == AttackPattern->NextComboEnable)
		{
			IsFinish = true;
		}
	}
	else
	{
		// 콤보 패턴을 찾지 못했을 경우 콤보 초기화
		PreviousAttackType = FGameplayTag::EmptyTag;
	}
}

FAttackPatternData* UAGGameplayAbility_ComboAttack::FindAttackPattern()
{
	if (false == IsValid(AttackPatternDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackPatternDataTable is not set in GGameplayAbility_ComboAttack."));
		return nullptr;
	}

	// 데이터 테이블에서 해당 패턴 찾기
	FString ContextString = TEXT("");
	TArray<FAttackPatternData*> AllRows;
	AttackPatternDataTable->GetAllRows<FAttackPatternData>(ContextString, AllRows);
	for (FAttackPatternData* Row : AllRows)
	{
		if (Row->PreviousAttackTypes.Find(PreviousAttackType) != INDEX_NONE && Row->InputType == InputAttackType)
		{
			return Row;
		}
	}

	return nullptr;
}

void UAGGameplayAbility_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo* ActorInfo,
                                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 데이터 테이블 없으면 채워주기
	if (false == IsValid(AttackPatternDataTable))
	{
		UAGTableData* TableData = UAGAssetManager::GetAssetByName<UAGTableData>("TableData");
		if (IsValid(TableData))
		{
			AttackPatternDataTable = TableData->FindDataTableByTag(AGGameplayTags::DataTable_AttackPattern);
		}
	}

	AGPlayerState = Cast<AAGPlayerState>(ActorInfo->OwnerActor);
	if (false == IsValid(AGPlayerState))
	{
		return;
	}

	// 첫 공격 입력은 외부에서 가져온다
	InputAttackType = AGPlayerState->GetInputAttackType();

	// 공격 중에 움직이지 못하게
	AGPlayerState->SetMovementState(AGGameplayTags::State_Movement_Block_Attack);

	AGPlayer = Cast<AAGPlayer>(ActorInfo->AvatarActor);

	// 공격 입력 이벤트를 기다림
	InputWaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Input.Action.Attack")));
	if (IsValid(InputWaitTask))
	{
		InputWaitTask->EventReceived.AddDynamic(this, &UAGGameplayAbility_ComboAttack::OnComboInputReceived);
		InputWaitTask->ReadyForActivation();
	}
	
	// 다음 공격으로 넘어갈 수 있는 시점에 활성화되는 이벤트를 기다림
	SaveAttackNotifyTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, AGGameplayTags::Event_Montage_SaveAttack);
	if (IsValid(SaveAttackNotifyTask))
	{
		SaveAttackNotifyTask->EventReceived.AddDynamic(this, &UAGGameplayAbility_ComboAttack::OnSaveAttackNotifyReceived);
		SaveAttackNotifyTask->ReadyForActivation();
	}

	// 공격 성공 이벤트를 기다림
	HitEnemyTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, AGGameplayTags::Event_Hit_Enemy);
	if (IsValid(HitEnemyTask))
	{
		HitEnemyTask->EventReceived.AddDynamic(this, &UAGGameplayAbility_ComboAttack::OnHitEnemyReceived);
		HitEnemyTask->ReadyForActivation();
	}

	// 공격 실행
	ExecuteComboAttack();
}

void UAGGameplayAbility_ComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 공격 타겟 초기화
	if (IsValid(AGPlayer))
	{
		AGPlayer->SetAttackTarget(nullptr);
	}

	// 움직임 막은 것 풀어줌
	if (IsValid(AGPlayerState))
	{
		AGPlayerState->SetMovementState(AGGameplayTags::State_Movement_Run);
	}
}

void UAGGameplayAbility_ComboAttack::OnMontageEnded(UAnimMontage* Montage, bool IsInterrupted)
{
	if (PreviousAttackType == FGameplayTag::EmptyTag)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UAGGameplayAbility_ComboAttack::OnComboInputReceived(FGameplayEventData Payload)
{
	if (false == IsValid(InputWaitTask))
	{
		return;
	}

	if (Payload.TargetTags.HasTag(AGGameplayTags::Input_Action_Attack_Light))
	{
		InputAttackType = EAttackType::Light;
	}
	else if (Payload.TargetTags.HasTag(AGGameplayTags::Input_Action_Attack_Heavy))
	{
		InputAttackType = EAttackType::Heavy;
	}
}

void UAGGameplayAbility_ComboAttack::OnSaveAttackNotifyReceived(FGameplayEventData Payload)
{
	// 다음 공격으로 넘어간다
	ExecuteComboAttack();
}

void UAGGameplayAbility_ComboAttack::OnHitEnemyReceived(FGameplayEventData Payload)
{
	if (false == IsValid(HitEnemyTask))
	{
		return;
	}

	if (false == IsValid(AGPlayer))
	{
		return;
	}
	
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (false == IsValid(AbilitySystemComponent))
	{
		return;
	}

	const AAGCharacter* Target = Cast<const AAGCharacter>(Payload.Target);
	if (false == IsValid(Target))
	{
		return;
	}

	UAbilitySystemComponent* TargetAbilitySystemComponent = Target->GetAbilitySystemComponent();
	if (false == IsValid(TargetAbilitySystemComponent))
	{
		return;
	}

	// AttackDamage Effect 생성
	TSubclassOf<UAGGameplayEffect_AttackDamage> DamageEffectClass = UAGGameplayEffect_AttackDamage::StaticClass();
	if (false == IsValid(DamageEffectClass))
	{
		return;
	}

	// 이펙트 컨텍스트 생성
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(AGPlayer);
	
	// 이펙트 스펙 생성
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, 1.f, EffectContextHandle);

	if (SpecHandle.IsValid())
	{
		// 이펙트 적용 (체력 차감)
		TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}

	// 타겟의 피격 어빌리티 활성화
	Target->ActivateAbility(AGGameplayTags::Ability_Hit);
	
	// 피격 애니메이션을 위해 타겟에게 이벤트 발생 시킴
	FGameplayEventData EventData;
	EventData.Instigator = AGPlayer; // 이벤트를 일으킨 캐릭터
	EventData.Target = Target; // 대상 캐릭터 (필요시 수정 가능)
	EventData.EventTag = FGameplayTag::RequestGameplayTag(FName("Animation.Attack"));
	EventData.TargetTags.AddTag(PreviousAttackType);
	
	TargetAbilitySystemComponent->HandleGameplayEvent(EventData.EventTag, &EventData);

	// 카메라 쉐이크
	AAGPlayerController* Controller = Cast<AAGPlayerController>(AGPlayer->GetController());
	if (IsValid(Controller))
	{
		Controller->PlayHitCameraShake();
	}

	// 슬로우 모션
	AGPlayer->StartSlowMotionAndGray(0.25f, 0.2f);
}
