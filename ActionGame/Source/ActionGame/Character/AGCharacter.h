// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AGCharacter.generated.h"

class UGameplayAbility;

UCLASS()
class ACTIONGAME_API AAGCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAGCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void InitAbilitySystem();
	virtual void HandleGameplayEvent(FGameplayTag EventTag);
	virtual void ActivateAbility(FGameplayTag AbilityTag) {}
	
	void AddCharacterAbilities();
	TSubclassOf<UGameplayAbility> FindAbility(FGameplayTag GameplayTag);

	void SetMovementStateTag(FGameplayTag Tag);
	FGameplayTag GetMovementStateTag();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAGAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category=Abilities)
	TMap<FGameplayTag ,TSubclassOf<UGameplayAbility>> StartupAbilities;

protected:
	// 움직임 상태 태그
	FGameplayTag MovementStateTag;
};
