// Fill out your copyright notice in the Description page of Project Settings.


#include "AGCharacter.h"
#include "ActionGame/AbilitySystem/AGAbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "ActionGame/AbilitySystem/Attributes/AGAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "../Widget/AGWidget_HpBar.h"

// Sets default values
AAGCharacter::AAGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	HpBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBarWidget"));

	if (IsValid(HpBarWidgetComponent))
	{
		HpBarWidgetComponent->SetupAttachment(RootComponent);
		HpBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		HpBarWidgetComponent->SetDrawAtDesiredSize(true);
		HpBarWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	}
}

// Called when the game starts or when spawned
void AAGCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 움직임 상태 태그 초기화
	MovementStateTag = FGameplayTag::RequestGameplayTag(FName("State.Movement.Idle"));

	RefreshHpBarRatio();
	AddCharacterAbilities();
}

// Called every frame
void AAGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent* AAGCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAGCharacter::InitAbilitySystem()
{
}

void AAGCharacter::HandleGameplayEvent(FGameplayTag EventTag)
{
}

void AAGCharacter::AddCharacterAbilities()
{
	UAGAbilitySystemComponent* ASC = Cast<UAGAbilitySystemComponent>(AbilitySystemComponent);
	if (false == IsValid(ASC))
	{
		return;
	}

	ASC->AddCharacterAbilities(StartupAbilities);
}

TSubclassOf<UGameplayAbility> AAGCharacter::FindAbility(FGameplayTag GameplayTag)
{
	return *StartupAbilities.Find(GameplayTag);
}

void AAGCharacter::PerformAttack(AAGCharacter* Target)
{
	if (false == IsValid(AbilitySystemComponent) || false == IsValid(Target))
	{
		return;
	}
	
	// 피격 애니메이션을 위해 타겟에게 이벤트 발생 시킴
	FGameplayEventData EventData;
	EventData.Instigator = this; // 이벤트를 일으킨 캐릭터
	EventData.Target = Target; // 대상 캐릭터 (필요시 수정 가능)
	EventData.EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Hit.Enemy"));
	
	AbilitySystemComponent->HandleGameplayEvent(EventData.EventTag, &EventData);
}

void AAGCharacter::SetMovementStateTag(FGameplayTag Tag)
{
	MovementStateTag = Tag;
}

FGameplayTag AAGCharacter::GetMovementStateTag()
{
	return MovementStateTag;
}

void AAGCharacter::BeginAttackTrace()
{
	CollStart = GetActorLocation() + GetActorForwardVector() * WeaponTraceRadius;
}

void AAGCharacter::EndAttackTrace()
{
	// 이전 위치와 현재 위치 사이를 트레이스로 검사
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	CollEnd = GetActorLocation() + GetActorForwardVector() * WeaponTraceRadius;

	// 트레이스 수행
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		CollStart,
		CollEnd,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(WeaponTraceRadius),
		Params
	);

	FVector Center = (CollStart + CollEnd) * 0.5f;
	float HalfHeight = ((Center - CollStart) * 0.5f).Length() + WeaponTraceRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat();
	FColor DrawColor;

	
	TArray<AActor*> HitActors;
	if (bHit)
	{
		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor && !HitActors.Contains(HitActor))
			{
				HitActors.Add(HitActor);

				// 데미지 적용 로직
				PerformAttack(Cast<AAGCharacter>(HitActor));
			}
		}

		DrawColor = FColor::Red;
	}
	else
	{
		DrawColor = FColor::Green;
	}

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, WeaponTraceRadius, Rotation, DrawColor, false, 0.5f);
}

void AAGCharacter::SetAttackTarget(AAGCharacter* Target)
{
	AttackTarget = Target;
}

AAGCharacter* AAGCharacter::GetAttackTarget()
{
	return AttackTarget;
}

void AAGCharacter::RefreshHpBarRatio()
{
	if (IsValid(HpBarWidgetComponent) && IsValid(AttributeSet))
	{
		float Hp = AttributeSet->GetHp();
		float MaxHp = AttributeSet->GetMaxHp();
		float Ratio = static_cast<float>(Hp) / MaxHp;
		UAGWidget_HpBar* HpBarWidget = Cast<UAGWidget_HpBar>(HpBarWidgetComponent->GetUserWidgetObject());
		HpBarWidget->SetHpRatio(Ratio);
	}
}
