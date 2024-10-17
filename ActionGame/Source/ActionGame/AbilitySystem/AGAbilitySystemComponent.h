// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AGAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// 캐릭터에 어빌리티를 부여함
	void AddCharacterAbilities(OUT const TMap<FGameplayTag,TSubclassOf<class UGameplayAbility>>& StartupAbilities);
	// 태그로 어빌리티 활성화
	void ActivateAbility(FGameplayTag AbilityTag);
	// 태그로 어빌리티 캔슬
	void CancelAbility(FGameplayTag AbilityTag);
	// 태그로 어빌리티 스펙 핸들 찾기
	FGameplayAbilitySpecHandle FindAbilitySpecHandle(FGameplayTag AbilityTag);

	// 부여한 어빌리티의 스펙 핸들들
	TArray<FGameplayAbilitySpecHandle> SpecHandles;
};
