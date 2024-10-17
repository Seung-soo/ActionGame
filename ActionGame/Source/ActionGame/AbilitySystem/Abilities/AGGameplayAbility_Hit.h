// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGGameplayAbility.h"
#include "AGGameplayAbility_Hit.generated.h"

struct FAttackPatternData;

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGGameplayAbility_Hit : public UAGGameplayAbility
{
	GENERATED_BODY()

public:
	UAGGameplayAbility_Hit();

public:
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
	
	// 콤보 패턴 데이터 찾기
	FAttackPatternData* FindAttackPattern(UDataTable* DataTable, FGameplayTag AttackType);

	// 애니메이션 몽타주 종료 시 호출되는 함수
	UFUNCTION()
	void OnMontageEnded();

	// 피격한 공격의 종류
	UFUNCTION()
	void OnAttackTypeReceived(FGameplayEventData Payload);

	void StartPushback();
	void PushbackTick();

private:
	FTimerHandle PushbackTimerHandle;
	float TotalPushbackDistance = 0.f;

	UPROPERTY()
	TObjectPtr<class AAGCharacter> MyCharacter = nullptr;
};
