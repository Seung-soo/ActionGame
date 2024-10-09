// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Player/AGPlayerController.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"
#include "../AbilitySystem/Attributes/AGPlayerSet.h"
#include "../Player/AGPlayerState.h"

AAGPlayer::AAGPlayer() : Super()
{
	// 컨트롤러 회전을 사용하도록 설정
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// 캐릭터 움직임 시 회전하지 않도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
}

void AAGPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AAGPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystem();
}

void AAGPlayer::InitAbilitySystem()
{
	Super::InitAbilitySystem();

	AAGPlayerState* PS = GetPlayerState<AAGPlayerState>();
	if (IsValid(PS))
	{
		AbilitySystemComponent = Cast<UAGAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		AttributeSet = PS->GetAGPlayerSet();
	}
}

void AAGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 현재 속도를 확인하여 움직임 상태 태그를 추가/제거
	FVector CurrentVelocity = GetVelocity();
	bool bIsMoving = !CurrentVelocity.IsNearlyZero();

	if (bIsMoving && !AbilitySystemComponent->HasMatchingGameplayTag(MovementStateTag))
	{
		AbilitySystemComponent->AddLooseGameplayTag(MovementStateTag);
	}
	else if (!bIsMoving && AbilitySystemComponent->HasMatchingGameplayTag(MovementStateTag))
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(MovementStateTag);
	}
}

void AAGPlayer::ActivateAbility(FGameplayTag AbilityTag)
{
	AbilitySystemComponent->ActivateAbility(AbilityTag);
}

USpringArmComponent* AAGPlayer::GetSpringArmComponent()
{
	return SpringArm;
}
