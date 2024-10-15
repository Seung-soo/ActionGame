// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGCharacter.h"
#include "AGPlayer.generated.h"

class USpringArmComponent;
class AAGMonster;

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
	virtual void HandleGameplayEvent(FGameplayTag EventTag) override;
	
public:
	virtual void ActivateAbility(FGameplayTag AbilityTag) const override;

	USpringArmComponent* GetSpringArmComponent();

	virtual void SelectAttackTarget() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USphereComponent> CollisionSphere;
};
