// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAnimInstance.h"

#include "ActionGame/AGGamplayTags.h"
#include "ActionGame/Character/AGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UAGAnimInstance::UAGAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UAGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
	Character = Cast<AAGCharacter>(TryGetPawnOwner());
	
	if (IsValid(Character))
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UAGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!IsValid(MovementComponent) || !IsValid(Character))
	{
		return;
	}

	Velocity = MovementComponent->Velocity;
	ShouldMove = (Velocity.Size2D() > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector);
}
