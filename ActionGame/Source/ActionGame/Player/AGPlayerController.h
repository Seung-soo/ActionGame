// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AGPlayerController.generated.h"

struct FInputActionValue;
class AAGCharacter;

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AAGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAGPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	void Input_Move(const FInputActionValue& InputValue);
	void Input_Roll(const FInputActionValue& InputValue);
	void Input_Block(const FInputActionValue& InputValue);
	void Input_WeakAttack(const FInputActionValue& InputValue);
	void Input_StrongAttack(const FInputActionValue& InputValue);
	void Input_Look(const FInputActionValue& InputValue);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AAGPlayer> AGPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MouseSensitivity_X = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MouseSensitivity_Y = 1.f;
};
