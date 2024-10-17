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

	// 가장 가까운 적 탐색
	virtual void SelectAttackTarget() override;
	
	// 구르는 모션 워핑에 필요한 타겟 Transform 추가
	void RollMotionWarping();
	// 카메라 거리 초기화
	void ResetTargetArmLength();
	// 구를 때 카메라 후진 시작
	void PlayRollCamera();
	// 후진된 카메라 원래 상태로 보간하면서 복구할 수 있게 값 입력
	void ResetCameraSetting();

	// 슬로우 모션 + 화면 무채색 전환 시작 
	void StartSlowMotionAndGray(float SlowDownFactor, float Duration);
	// 슬로우 모션 + 화면 무채색 전환 종료 
	void StopSlowMotionAndGray();
	void ResetPostProcessSettings();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> Camera;

	// 주변 타겟 공격 타겟 탐색용 컴포넌트
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

	// 기본 ArmLength
	float DefaultTargetArmLength = 300.f;

	// 구를 때 ArmLength
	float RollTargetArmLength = 500.f;

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