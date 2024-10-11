// Fill out your copyright notice in the Description page of Project Settings.


#include "AGMonster.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"
#include "../AbilitySystem/Attributes/AGMonsterSet.h"

AAGMonster::AAGMonster()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	AbilitySystemComponent = CreateDefaultSubobject<UAGAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UAGMonsterSet>("MonsterSet");
}

void AAGMonster::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilitySystem();
}

void AAGMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAGMonster::InitAbilitySystem()
{
	Super::InitAbilitySystem();

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AAGMonster::HandleGameplayEvent(FGameplayTag EventTag)
{
	Super::HandleGameplayEvent(EventTag);
}
