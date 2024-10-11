// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameplayAbility_Roll.h"

#include "ActionGame/AGGamplayTags.h"
#include "ActionGame/Player/AGPlayerController.h"
#include "ActionGame/Player/AGPlayerState.h"
#include "GameFramework/Character.h"

UAGGameplayAbility_Roll::UAGGameplayAbility_Roll()
{
	// 어빌리티의 실행 중지 여부 설정
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	// AbilityTags에 원하는 태그 추가
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Roll")));
}

void UAGGameplayAbility_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AAGPlayerController* Controller = Cast<AAGPlayerController>(ActorInfo->PlayerController);
	ACharacter* Player = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (false == IsValid(Controller) || false == IsValid(Player))
	{
		return;
	}

	UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
	if (false == IsValid(AnimInstance))
	{
		return;
	}
		
	FVector Direction = Controller->GetInputDirection();
	if (FVector::ZeroVector != Direction)
	{
		Player->SetActorRotation(Direction.Rotation());
	}
	Player->PlayAnimMontage(RollMontage);
		
	FOnMontageEnded MontageEnded;
	MontageEnded.BindUObject(this, &ThisClass::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEnded, RollMontage);

	AAGPlayerState* AGPlayerState = Cast<AAGPlayerState>(ActorInfo->OwnerActor);
	if (false == IsValid(AGPlayerState))
	{
		return;
	}

	AGPlayerState->SetMovementState(AGGameplayTags::State_Movement_Block_Roll);
}

void UAGGameplayAbility_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AAGPlayerState* AGPlayerState = Cast<AAGPlayerState>(ActorInfo->OwnerActor);
	if (false == IsValid(AGPlayerState))
	{
		return;
	}

	AGPlayerState->SetMovementState(AGGameplayTags::State_Movement_Idle);
}

void UAGGameplayAbility_Roll::OnMontageEnded(UAnimMontage* Montage, bool IsInterrupted)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
