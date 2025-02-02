// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameplayAbility_Hit.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimMontage.h"
#include "ActionGame/AGGamplayTags.h"
#include "ActionGame/Data/AGTableData.h"
#include "ActionGame/System/AGAssetManager.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ActionGame/AttackPatternData.h"
#include "ActionGame/Character/AGCharacter.h"

UAGGameplayAbility_Hit::UAGGameplayAbility_Hit()
{
	// 어빌리티의 실행 중지 여부 설정
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	// AbilityTags에 원하는 태그 추가
	AbilityTags.AddTag(AGGameplayTags::Ability_Hit);

	// 이미 활성화된 상태에서 또 활성화되지 않게
	BlockAbilitiesWithTag.AddTag(AGGameplayTags::Ability_ComboAttack);
	BlockAbilitiesWithTag.AddTag(AGGameplayTags::Ability_Roll);

	// 이 어빌리티가 활성화 될 때 취소할 어빌리티들
	CancelAbilitiesWithTag.AddTag(AGGameplayTags::Ability_Hit);
	CancelAbilitiesWithTag.AddTag(AGGameplayTags::Ability_ComboAttack);
}

void UAGGameplayAbility_Hit::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 피격 당한 공격 이벤트를 기다림
	UAbilityTask_WaitGameplayEvent* AttackTypeTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Animation.Attack")));
	if (IsValid(AttackTypeTask))
	{
		AttackTypeTask->EventReceived.AddDynamic(this, &UAGGameplayAbility_Hit::OnAttackTypeReceived);
		AttackTypeTask->ReadyForActivation();
	}

	MyCharacter = Cast<AAGCharacter>(ActorInfo->AvatarActor);

	// 체력 갱신
	if (IsValid(MyCharacter))
	{
		MyCharacter->RefreshHpBarRatio();
	}
}

void UAGGameplayAbility_Hit::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FAttackPatternData* UAGGameplayAbility_Hit::FindAttackPattern(UDataTable* DataTable, FGameplayTag AttackType)
{
	if (false == IsValid(DataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackPatternDataTable is not set in GGameplayAbility_Hit."));
		return nullptr;
	}

	// 데이터 테이블에서 해당 패턴 찾기
	FString ContextString = TEXT("");
	TArray<FAttackPatternData*> AllRows;
	DataTable->GetAllRows<FAttackPatternData>(ContextString, AllRows);
	for (FAttackPatternData* Row : AllRows)
	{
		if (Row->CurrentAttackType == AttackType)
		{
			return Row;
		}
	}

	return nullptr;
}

void UAGGameplayAbility_Hit::OnMontageEnded()
{
	GetWorld()->GetTimerManager().ClearTimer(PushbackTimerHandle); 
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAGGameplayAbility_Hit::OnAttackTypeReceived(FGameplayEventData Payload)
{
	if (Payload.TargetTags.IsEmpty())
	{
		return;
	}

	if (false == IsValid(MyCharacter))
	{
		return;
	}
	
	UAGTableData* TableData = UAGAssetManager::GetAssetByName<UAGTableData>("TableData");
	if (false == IsValid(TableData))
	{
		return;
	}
	
	UDataTable* AttackPatternDataTable = TableData->FindDataTableByTag(AGGameplayTags::DataTable_AttackPattern);
	if (false == IsValid(AttackPatternDataTable))
	{
		return;
	}

	FAttackPatternData* AttackPatternData = FindAttackPattern(AttackPatternDataTable, Payload.TargetTags.First());
	if (nullptr == AttackPatternData)
	{
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName(TEXT("MontageTask")), AttackPatternData->HitMontage); 
	if (false == IsValid(PlayMontageAndWaitTask))
	{
		return;
	}

	// 공격자 방향으로 회전
	FVector Dir = Payload.Instigator->GetActorLocation() - MyCharacter->GetActorLocation();
	MyCharacter->SetActorRotation(Dir.Rotation());

	// 뒤로 이동 시작
	StartPushback();

	// 몽타주가 종료되거나 캔슬, 중단되면 호출할 함수 바인딩
	PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageEnded);
	PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageEnded);
	PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageEnded);
	PlayMontageAndWaitTask->ReadyForActivation();
}

void UAGGameplayAbility_Hit::StartPushback()
{
	// 매 프레임마다 호출되는 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(PushbackTimerHandle, this, &UAGGameplayAbility_Hit::PushbackTick, GetWorld()->GetDeltaSeconds(), true);
}

void UAGGameplayAbility_Hit::PushbackTick()
{
	if (false == IsValid(MyCharacter))
	{
		return;
	}
	
	const float PushbackAmount = PushbackPower * GetWorld()->GetDeltaSeconds();
	TotalPushbackDistance += PushbackAmount;

	if (TotalPushbackDistance >= MaxPushbackDistance)
	{
		// 총 이동 거리가 최대 거리를 넘으면 타이머 중지
		GetWorld()->GetTimerManager().ClearTimer(PushbackTimerHandle);  
		return;
	}

	// 뒤 방향으로 이동
	FVector NewLocation = MyCharacter->GetActorLocation() - MyCharacter->GetActorForwardVector() * PushbackAmount;
	MyCharacter->SetActorLocation(NewLocation);
}
