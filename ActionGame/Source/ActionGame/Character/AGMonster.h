// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGCharacter.h"
#include "AGMonster.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AAGMonster : public AAGCharacter
{
	GENERATED_BODY()

public:
	AAGMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void InitAbilitySystem() override;
};
