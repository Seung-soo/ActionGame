// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGAttributeSet.h"
#include "../../AGAttackDefine.h"
#include "AGPlayerSet.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGPlayerSet : public UAGAttributeSet
{
	GENERATED_BODY()

	ATTRIBUTE_ACCESSORS(ThisClass, CurrentComboCount);

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Combo")
	FGameplayAttributeData CurrentComboCount;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Combo")
	EAttackType PreviousAttackType;
};
