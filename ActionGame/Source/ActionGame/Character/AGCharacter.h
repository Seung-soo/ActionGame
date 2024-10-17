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
	// 시작 어빌리티 부여
	void AddCharacterAbilities();
	// 공격 시에 충돌 성공했을 때 처리
	void PerformAttack(AAGCharacter* Target);
	
	void SetMovementStateTag(FGameplayTag Tag);
	FGameplayTag GetMovementStateTag();

	// 공격 충돌 감지
	void BeginAttackTrace();
	void EndAttackTrace();

	virtual void SelectAttackTarget() {};
	void SetAttackTarget(AAGCharacter* Target);
	AAGCharacter* GetAttackTarget();

	// Hp바 갱신
	void RefreshHpBarRatio();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Abilities)
	TObjectPtr<class UAGAttributeSet> AttributeSet;	// 스텟

	UPROPERTY(EditAnywhere, Category=Abilities)
	TMap<FGameplayTag ,TSubclassOf<UGameplayAbility>> StartupAbilities;	// 시작 시 바로 부여할 어빌리티

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> HpBarWidgetComponent;
	
	UPROPERTY()
	TObjectPtr<AAGCharacter> AttackTarget;	// 타겟

protected:
	FVector CollStart = FVector::ZeroVector;	// 충돌 Sweep 시작 지점
	FVector CollEnd = FVector::ZeroVector;		// 충돌 Sweep 마지막 지점
	float WeaponTraceRadius = 88.f;				// Sweep 원 반지름

protected:
	// 움직임 상태 태그
	FGameplayTag MovementStateTag;
};
