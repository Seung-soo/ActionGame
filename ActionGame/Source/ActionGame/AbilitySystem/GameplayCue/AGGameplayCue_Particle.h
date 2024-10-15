// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "AGGameplayCue_Particle.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGGameplayCue_Particle : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UAGGameplayCue_Particle();

public:
	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
