// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPlayerState.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"
#include "../AbilitySystem/Attributes/AGPlayerSet.h"
#include "ActionGame/Character/AGCharacter.h"

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

void AAGPlayerState::SetInputAttackType(EAttackType AttackType)
{
	InputAttackType = AttackType;
}

EAttackType AAGPlayerState::GetInputAttackType()
{
	return InputAttackType;
}

void AAGPlayerState::SetMovementState(FGameplayTag State)
{
	AAGCharacter* Character = Cast<AAGCharacter>(GetPawn());
	if (IsValid(Character))
	{
		Character->SetMovementStateTag(State);
	}
}