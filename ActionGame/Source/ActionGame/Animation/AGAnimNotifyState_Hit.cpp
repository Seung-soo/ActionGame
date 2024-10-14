// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAnimNotifyState_Hit.h"

#include "ActionGame/Character/AGCharacter.h"

void UAGAnimNotifyState_Hit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (IsValid(MeshComp))
	{
		AAGCharacter* Character = Cast<AAGCharacter>(MeshComp->GetOwner());
		if (IsValid(Character))
		{
			Character->BeginAttackTrace();
		}
	}
}

void UAGAnimNotifyState_Hit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (IsValid(MeshComp))
	{
		AAGCharacter* Character = Cast<AAGCharacter>(MeshComp->GetOwner());
		if (IsValid(Character))
		{
			Character->EndAttackTrace();
		}
	}
}
