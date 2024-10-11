// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AGAttackDefine.h"
#include "AttackPatternData.generated.h"

USTRUCT(BlueprintType)
struct FAttackPatternData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int32 ComboStep = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	EAttackType PreviousAttackType = EAttackType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	EAttackType CurrentInputType = EAttackType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool NextComboEnable = false;
};