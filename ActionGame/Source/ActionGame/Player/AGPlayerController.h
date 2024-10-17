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
	virtual void Tick(float DeltaSeconds) override;
	
private:
	void Input_Move(const FInputActionValue& InputValue);			// 움직임
	void Input_Roll(const FInputActionValue& InputValue);			// 구르기
	void Input_Block(const FInputActionValue& InputValue);			// 막기
	void Input_LightAttack(const FInputActionValue& InputValue);	// 약 공격
	void Input_HeavyAttack(const FInputActionValue& InputValue);	// 강 공격
	void Input_Look(const FInputActionValue& InputValue);			// 카메라 회전

public:
	void HandleGameplayEvent(FGameplayTag EventTag);
	void HandleGameplayEvent_SaveAttack();
	void HandleGameplayEvent_RollFinish();

public:
	void RotateCharacterToTarget();

	// 입력된 방향을 체크하는 함수
	FVector GetInputDirection();

	// 카메라 쉐이크 시작
	void PlayHitCameraShake();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AAGPlayer> AGPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;

	// 수평, 수직 감도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MouseSensitivity_X = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MouseSensitivity_Y = 1.f;
};
