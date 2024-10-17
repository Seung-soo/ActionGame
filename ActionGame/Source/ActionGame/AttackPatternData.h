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
	TArray<FGameplayTag> PreviousAttackTypes;	// 이전 공격 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	EAttackType InputType = EAttackType::None;	// 입력된 공격 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FGameplayTag CurrentAttackType = FGameplayTag::EmptyTag;	// 현재 공격 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;	// 공격 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UAnimMontage> HitMontage = nullptr;		// 피격 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool NextComboEnable = false;	// 이 후에 콤보 공격을 이어나갈 수 있는지
};