// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UAGAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AAGCharacter> Character;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

protected:
	// 속도
	UPROPERTY(BlueprintReadOnly)
	FVector Velocity = FVector::ZeroVector;

	// 움직일 수 있는지 여부
	UPROPERTY(BlueprintReadOnly)
	bool ShouldMove = false;
};
