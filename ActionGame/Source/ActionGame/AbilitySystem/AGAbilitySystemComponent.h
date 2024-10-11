// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AGAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AddCharacterAbilities(OUT const TMap<FGameplayTag,TSubclassOf<class UGameplayAbility>>& StartupAbilities);
	void ActivateAbility(FGameplayTag AbilityTag);
	void CancelAbility(FGameplayTag AbilityTag);
	UGameplayAbility* FindAbility(FGameplayTag AbilityTag);
	FGameplayAbilitySpecHandle FindAbilitySpecHandle(FGameplayTag AbilityTag);

	TArray<FGameplayAbilitySpecHandle> SpecHandles;
};
