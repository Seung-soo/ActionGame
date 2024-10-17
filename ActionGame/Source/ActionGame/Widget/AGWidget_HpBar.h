// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AGWidget_HpBar.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGWidget_HpBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHpRatio(float Ratio);

protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<class UProgressBar> HpBar;
};
