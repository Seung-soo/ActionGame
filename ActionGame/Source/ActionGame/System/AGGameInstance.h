// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UAGGameInstance(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init() override;
	virtual void Shutdown() override;
};
