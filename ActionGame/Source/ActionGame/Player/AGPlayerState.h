// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ActionGame/AGAttackDefine.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "AGPlayerState.generated.h"

class UAGGameplayAbility_ComboAttack;
class UAGAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AAGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAGPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAGAbilitySystemComponent* GetAGAbilitySystemComponent() const;
	class UAGPlayerSet* GetAGPlayerSet() const;
	
	void SetInputAttackType(EAttackType AttackType);
	EAttackType GetInputAttackType();

	void SetMovementState(FGameplayTag State);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAGPlayerSet> PlayerSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EAttackType InputAttackType;
};
