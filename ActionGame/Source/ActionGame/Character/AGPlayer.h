// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGCharacter.h"
#include "AGPlayer.generated.h"

class USpringArmComponent;

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AAGPlayer : public AAGCharacter
{
	GENERATED_BODY()

public:
	AAGPlayer();

public:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitAbilitySystem() override;
	virtual void Tick(float DeltaTime) override;

public:
	void ActivateAbility(FGameplayTag AbilityTag);

	USpringArmComponent* GetSpringArmComponent();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> Camera;
};
