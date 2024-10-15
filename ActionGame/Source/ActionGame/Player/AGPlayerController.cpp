// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPlayerController.h"

#include "AbilitySystemComponent.h"
#include "AGPlayerState.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActionGame/Data/AGInputData.h"
#include "ActionGame/System/AGAssetManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "../AGGamplayTags.h"
#include "../Character/AGPlayer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "ActionGame/AbilitySystem/AGAbilitySystemComponent.h"
#include "ActionGame/AbilitySystem/Abilities/AGGameplayAbility_ComboAttack.h"
#include "ActionGame/AbilitySystem/Attributes/AGPlayerSet.h"
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
		
		const UInputAction* ActionLightAttack = InputData->FindInputActionByTag(AGGameplayTags::Input_Action_Attack_Light);
		EnhancedInputComponent->BindAction(ActionLightAttack, ETriggerEvent::Started, this, &ThisClass::Input_LightAttack);

		const UInputAction* ActionHeavyAttack = InputData->FindInputActionByTag(AGGameplayTags::Input_Action_Attack_Heavy);
		EnhancedInputComponent->BindAction(ActionHeavyAttack, ETriggerEvent::Started, this, &ThisClass::Input_HeavyAttack);

		const UInputAction* ActionLook = InputData->FindInputActionByTag(AGGameplayTags::Input_Camera_Look);
		EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	}
}

void AAGPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AAGPlayerController::Input_Move(const FInputActionValue& InputValue)
{
	if (false == IsValid(AGPlayer))
	{
		return;
	}
	
	if (AGPlayer->GetMovementStateTag().MatchesTag(AGGameplayTags::State_Movement_Block))
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
 	if (AGPlayer->GetMovementStateTag().MatchesTag(AGGameplayTags::State_Movement_Block))
	{
		return;
	}
	
	if (IsValid(AGPlayer))
	{
		AGPlayer->ActivateAbility(AGGameplayTags::Ability_Roll);
	}
}

void AAGPlayerController::Input_Block(const FInputActionValue& InputValue)
{
}

void AAGPlayerController::Input_LightAttack(const FInputActionValue& InputValue)
{
	if (FGameplayTag::RequestGameplayTag(TEXT("State.Movement.Block.Roll")) == AGPlayer->GetMovementStateTag())
	{
		return;
	}

	AAGPlayerState* AGPlayerState = GetPlayerState<AAGPlayerState>();
	if (false == IsValid(AGPlayerState))
	{
		return;
	}

	if (AGPlayer->GetAbilitySystemComponent())
	{
		// GameplayEvent 데이터를 생성합니다.
		FGameplayEventData EventData;
		EventData.Instigator = AGPlayer;  // 이벤트를 일으킨 캐릭터
		EventData.Target = AGPlayer;      // 대상 캐릭터 (필요시 수정 가능)
		EventData.TargetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Input.Action.Attack.Light")));

		// 이벤트를 AbilitySystemComponent에서 트리거합니다.
		AGPlayer->GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Input.Action.Attack")), &EventData);
	}

	AGPlayerState->SetInputAttackType(EAttackType::Light);
	
	AGPlayer->ActivateAbility(AGGameplayTags::Ability_ComboAttack);
	
	AGPlayerState->SetInputAttackType(EAttackType::None);
}

void AAGPlayerController::Input_HeavyAttack(const FInputActionValue& InputValue)
{
	if (FGameplayTag::RequestGameplayTag(TEXT("State.Movement.Block.Roll")) == AGPlayer->GetMovementStateTag())
	{
		return;
	}

	if (false == IsValid(AGPlayer))
	{
		return;
	}
	
	AAGPlayerState* AGPlayerState = GetPlayerState<AAGPlayerState>();
	if (false == IsValid(AGPlayerState))
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = AGPlayer->GetAbilitySystemComponent();
	if (false == IsValid(AbilitySystemComponent))
	{
		return;
	}
	
	// GameplayEvent 데이터를 생성합니다.
	FGameplayEventData EventData;
	EventData.Instigator = this; // 이벤트를 일으킨 캐릭터
	EventData.Target = this; // 대상 캐릭터 (필요시 수정 가능)
	EventData.TargetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Input.Action.Attack.Heavy")));

	// 이벤트를 AbilitySystemComponent에서 트리거합니다.
	AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Input.Action.Attack")), &EventData);
	
	AGPlayerState->SetInputAttackType(EAttackType::Heavy);
	
	AGPlayer->ActivateAbility(AGGameplayTags::Ability_ComboAttack);

	AGPlayerState->SetInputAttackType(EAttackType::None);
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

void AAGPlayerController::HandleGameplayEvent(FGameplayTag EventTag)
{
	if (EventTag.MatchesTag(AGGameplayTags::Event_Montage_SaveAttack))
	{
		HandleGameplayEvent_SaveAttack();
	}
	else if (EventTag.MatchesTag(AGGameplayTags::Event_Montage_RollFinish))
	{
		HandleGameplayEvent_RollFinish();
	}
}

void AAGPlayerController::HandleGameplayEvent_SaveAttack()
{
	if (false == IsValid(AGPlayer))
	{
		return;
	}
	AAGPlayerState* AGPlayerState = GetPlayerState<AAGPlayerState>();
	if (false == IsValid(AGPlayerState))
	{
		return;
	}

	if (AGPlayer->GetAbilitySystemComponent())
	{
		// GameplayEvent 데이터를 생성합니다.
		FGameplayEventData EventData;
		EventData.Instigator = AGPlayer;  // 이벤트를 일으킨 캐릭터
		EventData.Target = AGPlayer;      // 대상 캐릭터 (필요시 수정 가능)

		// 이벤트를 AbilitySystemComponent에서 트리거합니다.
		AGPlayer->GetAbilitySystemComponent()->HandleGameplayEvent(AGGameplayTags::Event_Montage_SaveAttack, &EventData);
	}
	
	// UAGGameplayAbility_ComboAttack* ComboAttackAbility = AGPlayerState->GetAbilityComboAttack();
	//
	// if (IsValid(ComboAttackAbility) && true == ComboAttackAbility->IsActive())
	// {
	// 	ComboAttackAbility->DecideNextAttack();
	// }
}

void AAGPlayerController::HandleGameplayEvent_RollFinish()
{
	if (false == IsValid(AGPlayer))
	{
		return;
	}
	
	UAGAbilitySystemComponent* AbilitySystemComponent =  Cast<UAGAbilitySystemComponent>(AGPlayer->GetAbilitySystemComponent());
	if (false == IsValid(AbilitySystemComponent))
	{
		return;
	}

	AbilitySystemComponent->CancelAbility(AGGameplayTags::Ability_Roll);
}

void AAGPlayerController::RotateCharacterToTarget()
{
	if (false == IsValid(AGPlayer))
	{
		return;
	}

	FVector Direction = FVector::ZeroVector;

	AAGCharacter* Target = AGPlayer->GetAttackTarget();
	if (IsValid(Target))
	{
		// 타겟 방향으로 회전
		Direction = Target->GetActorLocation() - AGPlayer->GetActorLocation();
	}
	else
	{
		// 방향키 입력 방향으로 회전
		Direction = GetInputDirection();
	}

	if (FVector::ZeroVector != Direction)
	{
		AGPlayer->SetActorRotation(Direction.Rotation()); 
	}
}

FVector AAGPlayerController::GetInputDirection()
{
	FVector2D InputKey = FVector2D(0.0, 0.0);

	// 패드 방식도 고려한 듯한 코드
	// 패드 도입 시에는 이렇게 하면 안되긴 하다.
	double ForwardValue = 1.0;
	double RightValue = 1.0;
	
	if (IsInputKeyDown(EKeys::W))
	{
		InputKey.X += (1.0 * ForwardValue);
	}

	if (IsInputKeyDown(EKeys::S))
	{
		InputKey.X -= (1.0 * ForwardValue);
	}

	if (IsInputKeyDown(EKeys::A))
	{
		InputKey.Y -= (1.0 * RightValue);
	}

	if (IsInputKeyDown(EKeys::D))
	{
		InputKey.Y += (1.0 * RightValue);
	}

	// 플레이어 컨트롤러를 통해 카메라의 회전 값을 가져옴
	FRotator CameraRotation = GetControlRotation();

	// 카메라의 Yaw 값만 사용 (수평 회전)
	FRotator YawRotation(0.0f, CameraRotation.Yaw, 0.0f);

	FVector Direction = FVector(0.0, 0.0, 0.0);
	if (0.0 != InputKey.X)
	{
		Direction += (FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * InputKey.X);
	}
	if (0.0 != InputKey.Y)
	{
		Direction += (FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * InputKey.Y);
	}

	return Direction;
}
