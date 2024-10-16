// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGGameplayAbility.h"
#include "AGGameplayAbility_Roll.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGGameplayAbility_Roll : public UAGGameplayAbility
{
	GENERATED_BODY()

public:
	UAGGameplayAbility_Roll();

protected:
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

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category=Montage)
	TObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY()
	TObjectPtr<class AAGPlayer> Player;
};
