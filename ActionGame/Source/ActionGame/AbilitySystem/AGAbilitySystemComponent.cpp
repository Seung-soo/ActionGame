// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAbilitySystemComponent.h"

void UAGAbilitySystemComponent::AddCharacterAbilities(const TMap<FGameplayTag, TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (auto& AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass.Value, 1);
		FGameplayAbilitySpecHandle SpecHandle = GiveAbility(AbilitySpec);
		
		SpecHandles.Add(SpecHandle);
	}
}

void UAGAbilitySystemComponent::ActivateAbility(FGameplayTag AbilityTag)
{
	for	(const FGameplayAbilitySpecHandle& SpecHandle : SpecHandles)
	{
		FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle);
		if (nullptr == AbilitySpec || false == IsValid(AbilitySpec->Ability))
		{
			continue;
		}
		
		const FGameplayTagContainer& AbilityTags = AbilitySpec->Ability->AbilityTags;
		if (false == AbilityTags.HasTag(AbilityTag))
		{
			continue;
		}
		
		TryActivateAbility(SpecHandle);
	}
}

void UAGAbilitySystemComponent::CancelAbility(FGameplayTag AbilityTag)
{
	CancelAbilityHandle(FindAbilitySpecHandle(AbilityTag));
}

FGameplayAbilitySpecHandle UAGAbilitySystemComponent::FindAbilitySpecHandle(FGameplayTag AbilityTag)
{
	for	(const FGameplayAbilitySpecHandle& SpecHandle : SpecHandles)
	{
		FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle);
		if (nullptr == AbilitySpec || false == IsValid(AbilitySpec->Ability))
		{
			continue;
		}
		
		const FGameplayTagContainer& AbilityTags = AbilitySpec->Ability->AbilityTags;
		if (false == AbilityTags.HasTag(AbilityTag))
		{
			continue;
		}

		return SpecHandle;
	}

	return FGameplayAbilitySpecHandle();
}
