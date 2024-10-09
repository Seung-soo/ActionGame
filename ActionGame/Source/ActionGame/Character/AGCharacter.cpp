// Fill out your copyright notice in the Description page of Project Settings.


#include "AGCharacter.h"

#include "ActionGame/AbilitySystem/AGAbilitySystemComponent.h"

// Sets default values
AAGCharacter::AAGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAGCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 움직임 상태 태그 초기화
	MovementStateTag = FGameplayTag::RequestGameplayTag(FName("State.Movement.Idle"));

	AddCharacterAbilities();
}

// Called every frame
void AAGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* AAGCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAGCharacter::InitAbilitySystem()
{
}

void AAGCharacter::AddCharacterAbilities()
{
	UAGAbilitySystemComponent* ASC = Cast<UAGAbilitySystemComponent>(AbilitySystemComponent);
	if (false == IsValid(ASC))
	{
		return;
	}

	ASC->AddCharacterAbilities(StartupAbilities);
}

void AAGCharacter::SetMovementStateTag(FGameplayTag Tag)
{
	MovementStateTag = Tag;
}

FGameplayTag AAGCharacter::GetMovementStateTag()
{
	return MovementStateTag;
}
