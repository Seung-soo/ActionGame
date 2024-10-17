// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "AGGameplayEffect_AttackDamage.generated.h"

/* 공격했을 때 데미지를 주는 Gameplay Effect */

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGGameplayEffect_AttackDamage : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UAGGameplayEffect_AttackDamage();
	
};
