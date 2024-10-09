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
	void ExecuteComboAttack(EAttackType InputAttackType);

	// 콤보 패턴 데이터 찾기
	FAttackPatternData* FindAttackPattern(int32 ComboStep, EAttackType PrevAttack, EAttackType InputAttackType);

	// 다음 공격을 위한 어빌리티 활성화
	void ActivateNextAbility(int32 NextComboStep, EAttackType NextAttackType);

	// 콤보 초기화
	UFUNCTION()
	void ResetCombo();

	// 어빌리티 활성화 시 호출되는 함수
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	// 애니메이션 몽타주 종료 시 호출되는 함수
	UFUNCTION()
	void OnMontageEnded();

protected:
	// 데이터 테이블 참조
	UPROPERTY()
	UDataTable* AttackPatternDataTable;
	
	// 현재 콤보 단계
	int32 CurrentComboStep;

	// 이전 공격 타입
	EAttackType PreviousAttackType;

};
