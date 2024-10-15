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
	virtual void ActivateAbility(FGameplayTag AbilityTag) const {}

public:
	void AddCharacterAbilities();
	TSubclassOf<UGameplayAbility> FindAbility(FGameplayTag GameplayTag);

	void PerformAttack(AAGCharacter* Target);

	void SetMovementStateTag(FGameplayTag Tag);
	FGameplayTag GetMovementStateTag();

	void BeginAttackTrace();
	void EndAttackTrace();

	virtual void SelectAttackTarget() {};
	void SetAttackTarget(AAGCharacter* Target);
	AAGCharacter* GetAttackTarget();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Abilities)
	TObjectPtr<class UAGAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category=Abilities)
	TMap<FGameplayTag ,TSubclassOf<UGameplayAbility>> StartupAbilities;

	// 타겟
	UPROPERTY()
	TObjectPtr<AAGCharacter> AttackTarget;

protected:
	FVector CollStart = FVector::ZeroVector;
	FVector CollEnd = FVector::ZeroVector;
	float WeaponTraceRadius = 88.f;

protected:
	// 움직임 상태 태그
	FGameplayTag MovementStateTag;
};
