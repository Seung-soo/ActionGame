// Fill out your copyright notice in the Description page of Project Settings.


#include "AGCharacter.h"
#include "ActionGame/AbilitySystem/AGAbilitySystemComponent.h"
#include "ActionGame/AbilitySystem/Effects/AGGameplayEffect_AttackDamage.h"

// Sets default values
AAGCharacter::AAGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAGCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 움직임 상태 태그 초기화
	MovementStateTag = FGameplayTag::RequestGameplayTag(FName("State.Movement.Idle"));

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

	UAbilitySystemComponent* TargetAbilitySystemComponent = Target->GetAbilitySystemComponent();
	if (false == IsValid(TargetAbilitySystemComponent))
	{
		return;
	}

	// AttackDamage Effect 생성
	TSubclassOf<UAGGameplayEffect_AttackDamage> DamageEffectClass = UAGGameplayEffect_AttackDamage::StaticClass();
	if (false == IsValid(DamageEffectClass))
	{
		return;
	}

	// 이펙트 컨텍스트 생성
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	
	// 이펙트 스펙 생성
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, 1.f, EffectContextHandle);

	if (SpecHandle.IsValid())
	{
		// 이펙트 적용
		Target->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
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
	}
}
