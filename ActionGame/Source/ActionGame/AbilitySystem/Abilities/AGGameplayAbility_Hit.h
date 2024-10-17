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

	// 피격한 공격의 종류가 이벤트로 들어오는 함수
	UFUNCTION()
	void OnAttackTypeReceived(FGameplayEventData Payload);

	// 뒤로 밀려나게 타이머 시작
	void StartPushback();

	// 뒤로 밀려나는 타이머 동안 호출되는 Tick
	void PushbackTick();

private:
	// 밀려나는 타이머 핸들
	FTimerHandle PushbackTimerHandle;

	// 총 밀려난 거리
	float TotalPushbackDistance = 0.f;

	// 최대 밀려나는 거리
	float MaxPushbackDistance = 100.f;

	// 밀려나는 힘의 강도
	float PushbackPower = 200.f;
	

	UPROPERTY()
	TObjectPtr<class AAGCharacter> MyCharacter = nullptr;
};
