// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AGAttackDefine.h"
#include "AGGamplayTags.h"
#include "AttackPatternData.generated.h"

USTRUCT(BlueprintType)
struct FAttackPatternData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TArray<FGameplayTag> PreviousAttackTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	EAttackType InputType = EAttackType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FGameplayTag CurrentAttackType = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UAnimMontage> HitMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool NextComboEnable = false;
};