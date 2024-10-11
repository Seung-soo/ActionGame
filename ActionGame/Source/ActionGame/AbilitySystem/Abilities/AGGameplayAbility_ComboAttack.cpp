// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameplayAbility_ComboAttack.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"
#include "ActionGame/AGGamplayTags.h"
#include "GameplayEffect.h"
#include "ActionGame/AbilitySystem/Attributes/AGPlayerSet.h"
#include "ActionGame/Character/AGPlayer.h"
#include "ActionGame/Data/AGTableData.h"
#include "ActionGame/Player/AGPlayerState.h"
#include "ActionGame/System/AGAssetManager.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ActionGame/Player/AGPlayerController.h"

UAGGameplayAbility_ComboAttack::UAGGameplayAbility_ComboAttack()
{
	// 어빌리티의 실행 중지 여부 설정
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	// AbilityTags에 원하는 태그 추가
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.ComboAttack")));
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
	AvatarPlayer->StopAnimMontage();
	AvatarPlayer->PlayAnimMontage(MontageToPlay);

	FOnMontageEnded MontageEnded;
	MontageEnded.BindUObject(this, &ThisClass::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEnded, MontageToPlay);
}

void UAGGameplayAbility_ComboAttack::ExecuteComboAttack(EAttackType InputAttackType)
{
	// 콤보 패턴 데이터 찾기
	FAttackPatternData* AttackPattern = FindAttackPattern(CurrentComboStep, PreviousAttackType, InputAttackType);

	if (AttackPattern && false == IsFinish)
	{
		// 공격 애니메이션 재생
		PlayAttackMontage(AttackPattern->AttackMontage);
	
		// 콤보 상태 업데이트
		++CurrentComboStep;
		PreviousAttackType = InputAttackType;
		if (IsValid(AGPlayerState))
		{
			AGPlayerState->SetInputAttackType(EAttackType::None);
		}
		
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

		if (false == AttackPattern->NextComboEnable)
		{
			IsFinish = true;
		}
	}
	else
	{
		// 콤보 패턴을 찾지 못했을 경우 콤보 초기화
		PreviousAttackType = EAttackType::None;
	}
}

FAttackPatternData* UAGGameplayAbility_ComboAttack::FindAttackPattern(int32 ComboStep, EAttackType PrevAttack, EAttackType InputAttackType)
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
		if (Row->ComboStep == ComboStep &&
			Row->PreviousAttackType == PrevAttack &&
			Row->CurrentInputType == InputAttackType)
		{
			return Row;
		}
	}

	return nullptr;
}

void UAGGameplayAbility_ComboAttack::DecideNextAttack()
{
	if (false == IsValid(AGPlayerState))
	{
		return;
	}

	EAttackType InputAttackType = AGPlayerState->GetInputAttackType();
	
	ExecuteComboAttack(InputAttackType);
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

	AGPlayerState->SetAbilityComboAttack(this);
	AGPlayerState->SetMovementState(AGGameplayTags::State_Movement_Block_Attack);

	AGPlayer = Cast<AAGPlayer>(ActorInfo->AvatarActor);

	// 공격 실행
	DecideNextAttack();
}

void UAGGameplayAbility_ComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	CurrentComboStep = 1;
	PreviousAttackType = EAttackType::None;
	IsFinish = true;
	
	if (IsValid(AGPlayer))
	{
		AGPlayer->SetAttackTarget(nullptr);
	}

	if (IsValid(AGPlayerState))
	{
		AGPlayerState->SetInputAttackType(EAttackType::None);
		AGPlayerState->SetAbilityComboAttack(nullptr);
		AGPlayerState->SetMovementState(AGGameplayTags::State_Movement_Run);
	}
}

void UAGGameplayAbility_ComboAttack::OnMontageEnded(UAnimMontage* Montage, bool IsInterrupted)
{
	// 어빌리티 종료 시 콤보 타이머를 초기화하거나 추가적인 로직을 처리할 수 있습니다.
	// 현재 구현에서는 타이머가 콤보를 초기화하기 때문에 별도의 처리는 필요없습니다.
	if (IsValid(AGPlayerState) && PreviousAttackType == EAttackType::None)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}
