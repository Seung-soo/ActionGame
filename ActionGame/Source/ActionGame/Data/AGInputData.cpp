// Fill out your copyright notice in the Description page of Project Settings.


#include "AGInputData.h"
#include "InputAction.h"

const UInputAction* UAGInputData::FindInputActionByTag(const FGameplayTag& InputTag) const
{
	for (const FAGInputAction& Action : InputActions)
	{
		if (IsValid(Action.InputAction) && InputTag == Action.InputTag)
		{
			return Action.InputAction;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Can't find Input Action for InputTag [%s]"), *InputTag.ToString());

	return nullptr;
}
