// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAnimNotify_SaveAttack.h"
#include "../Character/AGCharacter.h"
#include "ActionGame/AGGamplayTags.h"

UAGAnimNotify_SaveAttack::UAGAnimNotify_SaveAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EventTag = AGGameplayTags::Event_Montage_SaveAttack;
}

void UAGAnimNotify_SaveAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AAGCharacter* LocalCharacter = Cast<AAGCharacter>(MeshComp->GetOwner());
	if (IsValid((LocalCharacter)))
	{
		LocalCharacter->HandleGameplayEvent(EventTag);
	}
}
