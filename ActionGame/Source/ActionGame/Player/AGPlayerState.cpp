// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPlayerState.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"
#include "../AbilitySystem/Attributes/AGPlayerSet.h"

AAGPlayerState::AAGPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UAGAbilitySystemComponent>("AbilitySystemComponent");
	PlayerSet = CreateDefaultSubobject<UAGPlayerSet>("PlayerSet");
}

UAbilitySystemComponent* AAGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAGAbilitySystemComponent* AAGPlayerState::GetAGAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAGPlayerSet* AAGPlayerState::GetAGPlayerSet() const
{
	return PlayerSet;
}