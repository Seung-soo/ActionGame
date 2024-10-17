// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameplayAbility_Roll.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "ActionGame/AGGamplayTags.h"
#include "ActionGame/Character/AGPlayer.h"
#include "ActionGame/Player/AGPlayerController.h"
#include "ActionGame/Player/AGPlayerState.h"

UAGGameplayAbility_Roll::UAGGameplayAbility_Roll()
{
	// 어빌리티의 실행 중지 여부 설정
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	// AbilityTags에 원하는 태그 추가
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Roll")));

	// 이미 활성화된 상태에서 또 활성화되지 않게
	BlockAbilitiesWithTag.AddTag(AGGameplayTags::Ability_ComboAttack);
	BlockAbilitiesWithTag.AddTag(AGGameplayTags::Ability_Roll);
	BlockAbilitiesWithTag.AddTag(AGGameplayTags::Ability_Hit);

	// 이 어빌리티가 활성화 될 때 취소할 어빌리티들
	CancelAbilitiesWithTag.AddTag(AGGameplayTags::Ability_ComboAttack);
}

void UAGGameplayAbility_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AAGPlayerController* Controller = Cast<AAGPlayerController>(ActorInfo->PlayerController);
	Player = Cast<AAGPlayer>(ActorInfo->AvatarActor);
	
	if (false == IsValid(Controller) || false == IsValid(Player))
	{
		return;
	}

	UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
	if (false == IsValid(AnimInstance))
	{
		return;
	}

	// 지금 입력되어 있는 방향을 가져와서 그 방향으로 회전
	FVector Direction = Controller->GetInputDirection();
	if (FVector::ZeroVector != Direction)
	{
		Player->SetActorRotation(Direction.Rotation());
	}

	// 몽타주 실행
	Player->PlayAnimMontage(RollMontage);

	// 모션 워핑으로 구르는 거리 늘려줌
	Player->RollMotionWarping();

	// 구를 때 카메라가 좀 뒤로가게 설정
	Player->PlayRollCamera();

	// 몽타주 종료됐을 때 호출될 함수 바인딩
	FOnMontageEnded MontageEnded;
	MontageEnded.BindUObject(this, &ThisClass::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEnded, RollMontage);

	AAGPlayerState* AGPlayerState = Cast<AAGPlayerState>(ActorInfo->OwnerActor);
	if (false == IsValid(AGPlayerState))
	{
		return;
	}

	// 구르는 동안에 움직이지 못하게
	AGPlayerState->SetMovementState(AGGameplayTags::State_Movement_Block);
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

	// 카메라 거리 원상 복구
	Player->ResetTargetArmLength();

	// 다시 움직일 수 있게
	AGPlayerState->SetMovementState(AGGameplayTags::State_Movement_Idle);
}

void UAGGameplayAbility_Roll::OnMontageEnded(UAnimMontage* Montage, bool IsInterrupted)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}