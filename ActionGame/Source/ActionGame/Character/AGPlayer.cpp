// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPlayer.h"
#include "AGMonster.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Player/AGPlayerController.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"
#include "../AbilitySystem/Attributes/AGPlayerSet.h"
#include "../Player/AGPlayerState.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

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
		SpringArm->TargetArmLength = CurrentTargetArmLength;
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

	// 빙의 됐을 때 어빌리티 시스템 초기화
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
	
	if (IsValid(SpringArm))
	{
		// TargetArmLength 보간
		CurrentTargetArmLength = FMath::FInterpTo(CurrentTargetArmLength, TargetArmLength, DeltaTime, ArmLengthInterpSpeed);
		SpringArm->TargetArmLength = CurrentTargetArmLength;
	}

	if (IsValid(Camera))
	{
		// 채도 값 보간
		CurrentSaturation = FMath::FInterpTo(CurrentSaturation, TargetSaturation, DeltaTime, SaturationInterpSpeed);

		// 카메라의 포스트 프로세싱 설정 업데이트
		FPostProcessSettings& PostProcessSettings = Camera->PostProcessSettings;
		if (PostProcessSettings.bOverride_ColorSaturation)
		{
			PostProcessSettings.ColorSaturation = FVector4(CurrentSaturation, CurrentSaturation, CurrentSaturation, 1.0f);
		}
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

	// 가장 가까운 적 저장
	AttackTarget = Cast<AAGCharacter>(ClosestMonster);
}

void AAGPlayer::RollMotionWarping()
{
	if (false == IsValid(MotionWarpingComponent))
	{
		return;
	}

	// 모션 워핑을 할 타겟 Transform 추가
	FVector TargetLocation = GetActorLocation() + GetActorForwardVector() * DesiredRollDistance;
	FTransform MyTransform = GetTransform();
	MyTransform.SetLocation(TargetLocation);
	MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(FName("RollTarget"), MyTransform);
}

void AAGPlayer::ResetTargetArmLength()
{
	if (IsValid(SpringArm))
	{
		SpringArm->TargetArmLength = DefaultTargetArmLength;
	}
}

void AAGPlayer::PlayRollCamera()
{
	TargetArmLength = RollTargetArmLength;
	GetWorldTimerManager().SetTimer(RollTimerHandle, this, &AAGPlayer::ResetCameraSetting, 0.2f, false);
}

void AAGPlayer::ResetCameraSetting()
{
	TargetArmLength = DefaultTargetArmLength;
}

void AAGPlayer::StartSlowMotionAndGray(float SlowDownFactor, float Duration)
{
	// 시간 지연 설정
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowDownFactor);

	// 슬로우 모션 종료를 위한 타이머 설정
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AAGPlayer::StopSlowMotionAndGray, Duration * SlowDownFactor, false);

	if (IsValid(Camera))
	{
		// 카메라의 포스트 프로세싱 설정에서 채도(Saturation)를 조절
		FPostProcessSettings& PostProcessSettings = Camera->PostProcessSettings;
		PostProcessSettings.bOverride_ColorSaturation = true;
		PostProcessSettings.ColorSaturation = FVector4(0.0f, 0.0f, 0.0f, 1.0f); // RGB 채도를 0으로 설정하여 무채색으로 만듦
		TargetSaturation = 0.0f; // 무채색으로 목표 설정
	}
}

void AAGPlayer::StopSlowMotionAndGray()
{
	// 시간 지연을 원래대로 복구
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

	if (IsValid(Camera))
	{
		// 카메라의 포스트 프로세싱 설정 복구
		FPostProcessSettings& PostProcessSettings = Camera->PostProcessSettings;
		PostProcessSettings.ColorSaturation = FVector4(1.0f, 1.0f, 1.0f, 1.0f); // RGB 채도를 원래대로 복구
		TargetSaturation = 1.0f; // 원래 색상으로 복귀
	}

	// 슬로우 모션 종료 후 일정 시간 후에 포스트 프로세싱 오버라이드를 해제
	FTimerHandle PostProcessTimerHandle;
	GetWorldTimerManager().SetTimer(PostProcessTimerHandle, this, &AAGPlayer::ResetPostProcessSettings, 0.2f, false);
}

void AAGPlayer::ResetPostProcessSettings()
{
	if (IsValid(Camera))
	{
		Camera->PostProcessSettings.bOverride_ColorSaturation = false;
	}
}
