// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPlayer.h"

#include "AGMonster.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Player/AGPlayerController.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"
#include "../AbilitySystem/Attributes/AGPlayerSet.h"
#include "../Player/AGPlayerState.h"
#include "Components/SphereComponent.h"

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
	if (IsValid(SpringArm))
	{
		SpringArm->SetupAttachment(GetCapsuleComponent());
		SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
		SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
		SpringArm->TargetArmLength = 300.0f;
		SpringArm->bUsePawnControlRotation = true;
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (IsValid(Camera))
	{
		Camera->SetupAttachment(SpringArm);
		Camera->bUsePawnControlRotation = false;
	}
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	if (IsValid(CollisionSphere))
	{
		CollisionSphere->SetupAttachment(GetCapsuleComponent());
		CollisionSphere->SetSphereRadius(1000.f);
		CollisionSphere->SetHiddenInGame(false);
		CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	}

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
		AbilitySystemComponent = PS->GetAGAbilitySystemComponent();
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

void AAGPlayer::HandleGameplayEvent(FGameplayTag EventTag)
{
	Super::HandleGameplayEvent(EventTag);
	
	AAGPlayerController* PC = Cast<AAGPlayerController>(GetController());
	if (IsValid(PC))
	{
		PC->HandleGameplayEvent(EventTag);
	}
}

void AAGPlayer::ActivateAbility(FGameplayTag AbilityTag) const
{
	Super::ActivateAbility(AbilityTag);
	
	AbilitySystemComponent->ActivateAbility(AbilityTag);
}

USpringArmComponent* AAGPlayer::GetSpringArmComponent()
{
	return SpringArm;
}

void AAGPlayer::SelectAttackTarget()
{
	Super::SelectAttackTarget();
	
	if (false == IsValid(CollisionSphere))
	{
		return;
	}
	
	TArray<AActor*> OverlappingActors;
	// 일정 반경 내의 모든 적을 검색
	CollisionSphere->GetOverlappingActors(OverlappingActors, AAGMonster::StaticClass());

	AAGMonster* ClosestMonster = nullptr;
	float MinDistance = FLT_MAX;
	FVector PlayerLocation = GetActorLocation();

	for (AActor* Actor : OverlappingActors)
	{
		float Distance = FVector::Dist(PlayerLocation, Actor->GetActorLocation());
		if (Distance >= MinDistance)
		{
			continue;
		}

		AAGMonster* Monster = Cast<AAGMonster>(Actor);
		if (IsValid(Monster))
		{
			MinDistance = Distance;
			ClosestMonster = Monster;
		}
	}

	AttackTarget = Cast<AAGCharacter>(ClosestMonster);
}