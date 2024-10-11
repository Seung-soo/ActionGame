// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AGAnimNotify_SaveAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGAnimNotify_SaveAttack : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAGAnimNotify_SaveAttack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag EventTag;
};
