// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGAttackDefine.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	None        UMETA(DisplayName = "None"),
	Light       UMETA(DisplayName = "Light"),
	Heavy       UMETA(DisplayName = "Heavy")
};

USTRUCT(BlueprintType)
struct FAttackCombo
{
	GENERATED_BODY()

	// 현재 콤보 단계
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int32 ComboStep;

	// 이전 공격 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	EAttackType PreviousAttackType;

	// 현재 입력 타입 (이번 공격)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	EAttackType CurrentInputType;

	// 다음 공격 타입 (다음 공격 시 필요한 타입)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	EAttackType NextAttackType;

	// 실행할 공격 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* AttackMontage;

	// 다음 공격을 위한 콤보 단계 (다음 단계로 넘어갈 경우)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int32 NextComboStep;
};