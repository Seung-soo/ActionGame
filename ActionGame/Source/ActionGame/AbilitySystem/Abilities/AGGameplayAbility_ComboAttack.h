// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGGameplayAbility.h"
#include "../../AGAttackDefine.h"
#include "ActionGame/AttackPatternData.h"
#include "AGGameplayAbility_ComboAttack.generated.h"

class AAGPlayerState;
class AAGPlayer;
class UAbilityTask_WaitGameplayEvent;

/* 콤보 공격 어빌리티 클래스 */

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

	// 공격 중에 공격 입력이 들어왔을 때 이벤트 호출되는 함수 (입력을 저장함)
	UFUNCTION()
	void OnComboInputReceived(FGameplayEventData Payload);

	// 다음 공격으로 넘어갈 수 있는 시점에 노티파이를 통해서 건너 건너 이벤트 호출
	UFUNCTION()
	void OnSaveAttackNotifyReceived(FGameplayEventData Payload);

	// 공격이 성공(충돌)이 감지되었을 때 이벤트 호출
	UFUNCTION()
	void OnHitEnemyReceived(FGameplayEventData Payload);

protected:
	// 다음 공격에 대한 데이터 테이블
	UPROPERTY()
	TObjectPtr<UDataTable> AttackPatternDataTable = nullptr;

	UPROPERTY()
	TObjectPtr<AAGPlayerState> AGPlayerState = nullptr;

	UPROPERTY()
	TObjectPtr<AAGPlayer> AGPlayer = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> InputWaitTask = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> SaveAttackNotifyTask = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> HitEnemyTask = nullptr;

	// 이전 공격 타입
	FGameplayTag PreviousAttackType = FGameplayTag::EmptyTag;

	// 입력 받은 공격 타입
	EAttackType InputAttackType = EAttackType::None;

	bool IsFinish = false;
};
