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

	void RollMotionWarping();

	void SetTargetArmLength(float Length);

	void PlayRollCamera();
	void ResetCameraSetting();

	void StartSlowMotion(float SlowDownFactor, float Duration);
	void StopSlowMotion();
	void ResetPostProcessSettings();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USphereComponent> CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DesiredRollDistance = 500.f;

	// 구르기 카메라 보간 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ArmLengthInterpSpeed = 8.f;

	// 카메라 타겟 거리 (현재 값)
	float CurrentTargetArmLength = 300.f;

	// 타겟 ArmLength (목표 값)
	float TargetArmLength = 300.f;

	// 채도 보간 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float SaturationInterpSpeed = 50.0f;

	// 현재 채도 값
	float CurrentSaturation = 1.f;

	// 목표 채도 값
	float TargetSaturation = 1.f;

	FTimerHandle RollTimerHandle = {};
	FTimerHandle SlowMotionimerHandle = {};
};