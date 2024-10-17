// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAnimNotifyState_Hit.h"

#include "ActionGame/Character/AGCharacter.h"

void UAGAnimNotifyState_Hit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	// Notify State의 시작 지점에 들어옴
	// 공격 충돌 탐지 시작 지점 저장
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

	// Notify State의 끝 지점에 들어옴
	// 충돌 처리
	if (IsValid(MeshComp))
	{
		AAGCharacter* Character = Cast<AAGCharacter>(MeshComp->GetOwner());
		if (IsValid(Character))
		{
			Character->EndAttackTrace();
		}
	}
}
