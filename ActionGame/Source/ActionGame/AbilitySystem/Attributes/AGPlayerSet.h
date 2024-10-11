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

public:
	UAGPlayerSet();
	
public:
	ATTRIBUTE_ACCESSORS(ThisClass, Stamina);
	ATTRIBUTE_ACCESSORS(ThisClass, MaxStamina);

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxStamina;
};
