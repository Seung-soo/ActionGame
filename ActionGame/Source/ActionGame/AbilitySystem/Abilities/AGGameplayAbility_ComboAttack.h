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

protected:
	// 공격 애니메이션 몽타주 재생
	void PlayAttackMontage(UAnimMontage* MontageToPlay);

	// 콤보 패턴 결정 및 공격 실행
	void ExecuteComboAttack();

	// 콤보 패턴 데이터 찾기
	FAttackPatternData* FindAttackPattern();
	
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

	UFUNCTION()
	void OnComboInputReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnSaveAttackNotifyReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnHitEnemyReceived(FGameplayEventData Payload);

protected:
	// 데이터 테이블 참조
	UPROPERTY()
	TObjectPtr<UDataTable> AttackPatternDataTable = nullptr;

	UPROPERTY()
	TObjectPtr<class AAGPlayerState> AGPlayerState = nullptr;

	UPROPERTY()
	TObjectPtr<class AAGPlayer> AGPlayer = nullptr;

	UPROPERTY()
	TObjectPtr<class UAbilityTask_WaitGameplayEvent> InputWaitTask = nullptr;
	
	UPROPERTY()
	TObjectPtr<class UAbilityTask_WaitGameplayEvent> SaveAttackNotifyTask = nullptr;

	UPROPERTY()
	TObjectPtr<class UAbilityTask_WaitGameplayEvent> HitEnemyTask = nullptr;

	// 이전 공격 타입
	FGameplayTag PreviousAttackType = FGameplayTag::EmptyTag;

	// 입력 받은 공격 타입
	EAttackType InputAttackType = EAttackType::None;

	bool IsFinish = false;
};
