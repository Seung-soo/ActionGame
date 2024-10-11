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