// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActionGame/Data/AGInputData.h"
#include "ActionGame/System/AGAssetManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "../AGGamplayTags.h"
#include "../Character/AGPlayer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/SpringArmComponent.h"

AAGPlayerController::AAGPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AAGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (const UAGInputData* InputData = UAGAssetManager::GetAssetByName<UAGInputData>("InputData"))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (IsValid(Subsystem))
		{
			Subsystem->AddMappingContext(InputData->InputMappingContext, 0);
		}
	}

	AGPlayer = Cast<AAGPlayer>(GetCharacter());

	if (IsValid(PlayerCameraManager))
	{
		PlayerCameraManager->ViewPitchMin = -60.0f;
		PlayerCameraManager->ViewPitchMax = 30.0f;
	}
}

void AAGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (const UAGInputData* InputData = UAGAssetManager::GetAssetByName<UAGInputData>("InputData"))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
		
		const UInputAction* ActionMove = InputData->FindInputActionByTag(AGGameplayTags::Input_Action_Move);
		EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		
		const UInputAction* ActionRoll = InputData->FindInputActionByTag(AGGameplayTags::Input_Action_Roll);
		EnhancedInputComponent->BindAction(ActionRoll, ETriggerEvent::Started, this, &ThisClass::Input_Roll);
		
		const UInputAction* ActionBlock = InputData->FindInputActionByTag(AGGameplayTags::Input_Action_Block);
		EnhancedInputComponent->BindAction(ActionBlock, ETriggerEvent::Started, this, &ThisClass::Input_Block);
		
		const UInputAction* ActionWeakAttack = InputData->FindInputActionByTag(AGGameplayTags::Input_Action_WeakAttack);
		EnhancedInputComponent->BindAction(ActionWeakAttack, ETriggerEvent::Started, this, &ThisClass::Input_WeakAttack);

		const UInputAction* ActionStrongAttack = InputData->FindInputActionByTag(AGGameplayTags::Input_Action_StrongAttack);
		EnhancedInputComponent->BindAction(ActionStrongAttack, ETriggerEvent::Started, this, &ThisClass::Input_StrongAttack);

		const UInputAction* ActionLook = InputData->FindInputActionByTag(AGGameplayTags::Input_Camera_Look);
		EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	}
}

void AAGPlayerController::Input_Move(const FInputActionValue& InputValue)
{
	if (!IsValid(AGPlayer))
	{
		return;
	}
	
	FVector2D MovementVector = InputValue.Get<FVector2D>();
	
	FRotator ControllerRotator = GetDesiredRotation();

	if (MovementVector.X != 0)
	{
		FVector ControllerDirection = UKismetMathLibrary::GetForwardVector(FRotator(0, ControllerRotator.Yaw, 0));
		GetPawn()->AddMovementInput(ControllerDirection, MovementVector.X);
	}
	
	if (MovementVector.Y != 0)
	{
		FVector ControllerDirection = UKismetMathLibrary::GetRightVector(FRotator(0, ControllerRotator.Yaw, 0));
		GetPawn()->AddMovementInput(ControllerDirection, MovementVector.Y);
	}
}

void AAGPlayerController::Input_Roll(const FInputActionValue& InputValue)
{
}

void AAGPlayerController::Input_Block(const FInputActionValue& InputValue)
{
}

void AAGPlayerController::Input_WeakAttack(const FInputActionValue& InputValue)
{

}

void AAGPlayerController::Input_StrongAttack(const FInputActionValue& InputValue)
{
}

void AAGPlayerController::Input_Look(const FInputActionValue& InputValue)
{
	if (!IsValid(AGPlayer))
	{
		return;
	}

	// 프레임 시간에 따른 입력 보정
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	const FVector2D& LookAxisVector = InputValue.Get<FVector2D>();

	// 마우스 감도 조절을 위해 Scale 값 적용 가능
	AGPlayer->AddControllerYawInput(LookAxisVector.X * MouseSensitivity_X * DeltaTime);
	AGPlayer->AddControllerPitchInput(-LookAxisVector.Y * MouseSensitivity_Y * DeltaTime);
}