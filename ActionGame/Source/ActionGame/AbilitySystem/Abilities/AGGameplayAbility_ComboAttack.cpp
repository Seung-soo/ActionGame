// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameplayAbility_ComboAttack.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"
#include "GameplayEffect.h"

UAGGameplayAbility_ComboAttack::UAGGameplayAbility_ComboAttack()
{
	// 어빌리티의 실행 중지 여부 설정
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAGGameplayAbility_ComboAttack::PlayAttackMontage(UAnimMontage* MontageToPlay)
{
	if (MontageToPlay)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, MontageToPlay, 1.0f, NAME_None, false);

		MontageTask->OnCompleted.AddDynamic(this, &UAGGameplayAbility_ComboAttack::OnMontageEnded);
		MontageTask->OnInterrupted.AddDynamic(this, &UAGGameplayAbility_ComboAttack::OnMontageEnded);
		MontageTask->OnCancelled.AddDynamic(this, &UAGGameplayAbility_ComboAttack::OnMontageEnded);

		MontageTask->ReadyForActivation();
	}
}

void UAGGameplayAbility_ComboAttack::ExecuteComboAttack(EAttackType InputAttackType)
{
	// 콤보 패턴 데이터 찾기
	FAttackPatternData* AttackPattern = FindAttackPattern(CurrentComboStep, PreviousAttackType, InputAttackType);

	if (AttackPattern)
	{
		// 공격 애니메이션 재생
		PlayAttackMontage(AttackPattern->AttackMontage);

		// 콤보 상태 업데이트
		++CurrentComboStep;
		PreviousAttackType = InputAttackType;
		
		FTimerHandle TimerHandle;
		// 콤보 타이머 설정 (예시: 1초 후 콤보 초기화)
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&UAGGameplayAbility_ComboAttack::ResetCombo,
			1.0f, // 콤보 유지 시간 (예시)
			false
		);
	}
	else
	{
		// 콤보 패턴을 찾지 못했을 경우 콤보 초기화
		ResetCombo();
	}
}

FAttackPatternData* UAGGameplayAbility_ComboAttack::FindAttackPattern(int32 ComboStep, EAttackType PrevAttack,
	EAttackType InputAttackType)
{
	if (!AttackPatternDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackPatternDataTable is not set in UGA_ComboAttack."));
		return nullptr;
	}

	// 데이터 테이블에서 해당 패턴 찾기
	FString ContextString;
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

void UAGGameplayAbility_ComboAttack::ActivateNextAbility(int32 NextComboStep, EAttackType NextAttackType)
{
	// 현재 어빌리티 클래스에서 다음 어빌리티를 활성화하는 대신, 콤보 단계와 공격 타입을 업데이트
	// 다음 공격을 위해 새로운 어빌리티 인스턴스를 생성하거나, 동일한 어빌리티를 재활성화할 수 있습니다.
	// 여기서는 단일 어빌리티 클래스를 사용하므로, 추가 어빌리티 활성화는 필요없습니다.
}

void UAGGameplayAbility_ComboAttack::ResetCombo()
{
	// 콤보 초기화 로직
	CurrentComboStep = 1;
	PreviousAttackType = EAttackType::None;
}

void UAGGameplayAbility_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAGGameplayAbility_ComboAttack::OnMontageEnded()
{
	// 어빌리티 종료 시 콤보 타이머를 초기화하거나 추가적인 로직을 처리할 수 있습니다.
	// 현재 구현에서는 타이머가 콤보를 초기화하기 때문에 별도의 처리는 필요없습니다.
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
