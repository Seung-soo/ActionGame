// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGGameplayAbility.h"
#include "../../AGAttackDefine.h"
#include "ActionGame/AttackPatternData.h"
#include "AGGameplayAbility_ComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGGameplayAbility_ComboAttack : public UAGGameplayAbility
{
	GENERATED_BODY()
	
public:
	UAGGameplayAbility_ComboAttack();

public:
	// 다음 공격을 실행하거나 다음 공격을 실행하지 않고 애니메이션 종료까지 기다릴지 결정하는 함수
	void DecideNextAttack();

protected:
	// 공격 애니메이션 몽타주 재생
	void PlayAttackMontage(UAnimMontage* MontageToPlay);

	// 콤보 패턴 결정 및 공격 실행
	void ExecuteComboAttack(EAttackType InputAttackType);

	// 콤보 패턴 데이터 찾기
	FAttackPatternData* FindAttackPattern(int32 ComboStep, EAttackType PrevAttack, EAttackType InputAttackType);
	
	// 어빌리티 활성화 시 호출되는 함수
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	// 어빌리티 종료 시 호출되는 함수
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
							const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo,
							bool bReplicateEndAbility,
							bool bWasCancelled) override;

	// 애니메이션 몽타주 종료 시 호출되는 함수
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool IsInterrupted);

protected:
	// 데이터 테이블 참조
	UPROPERTY()
	TObjectPtr<UDataTable> AttackPatternDataTable;

	UPROPERTY()
	TObjectPtr<class AAGPlayerState> AGPlayerState;

	UPROPERTY()
	TObjectPtr<class AAGPlayer> AGPlayer;

	// 현재 콤보 단계
	int32 CurrentComboStep = 1;

	// 이전 공격 타입
	EAttackType PreviousAttackType = EAttackType::None;

	bool IsFinish = false;
};
