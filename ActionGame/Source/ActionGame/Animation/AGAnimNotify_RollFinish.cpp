// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAnimNotify_RollFinish.h"
#include "../Character/AGCharacter.h"
#include "ActionGame/AGGamplayTags.h"
#include "ActionGame/Player/AGPlayerController.h"

UAGAnimNotify_RollFinish::UAGAnimNotify_RollFinish(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EventTag = AGGameplayTags::Event_Montage_RollFinish;
}

void UAGAnimNotify_RollFinish::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 구르고 발이 땅에 붙었을 때 Notify가 들어와서 Event_Montage_RollFinish태그로 이벤트를 보냄
	AAGCharacter* LocalCharacter = Cast<AAGCharacter>(MeshComp->GetOwner());
	if (IsValid((LocalCharacter)))
	{
		LocalCharacter->HandleGameplayEvent(EventTag);
	}
}
