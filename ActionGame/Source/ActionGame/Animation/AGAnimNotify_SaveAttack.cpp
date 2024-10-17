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

	// 다음 공격으로 이어갈 수 있는 타이밍에 들어온 Notify
	// Event_Montage_SaveAttack 이벤트를 보내줌
	AAGCharacter* LocalCharacter = Cast<AAGCharacter>(MeshComp->GetOwner());
	if (IsValid((LocalCharacter)))
	{
		LocalCharacter->HandleGameplayEvent(EventTag);
	}
}
