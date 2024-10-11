// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAnimNotify_RollFinish.h"
#include "../Character/AGCharacter.h"
#include "ActionGame/AGGamplayTags.h"

UAGAnimNotify_RollFinish::UAGAnimNotify_RollFinish(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EventTag = AGGameplayTags::Event_Montage_RollFinish;
}

void UAGAnimNotify_RollFinish::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AAGCharacter* LocalCharacter = Cast<AAGCharacter>(MeshComp->GetOwner());
	if (IsValid((LocalCharacter)))
	{
		LocalCharacter->HandleGameplayEvent(EventTag);
	}
}
