// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameplayEffect_AttackDamage.h"

#include "ActionGame/AbilitySystem/Attributes/AGAttributeSet.h"

UAGGameplayEffect_AttackDamage::UAGGameplayEffect_AttackDamage()
{
	// 즉시 효과로 설정
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// Modifier 설정
	FGameplayModifierInfo DamageModifierInfo;

	// 대상의 Hp 속성을 수정할 것임을 알림
	DamageModifierInfo.Attribute = UAGAttributeSet::GetHpAttribute();

	// Modifier Operation은 Additive (값을 더하거나 뺌)
	DamageModifierInfo.ModifierOp = EGameplayModOp::Additive;

	// AttributeBasedFloat 설정
	FAttributeBasedFloat AttributeBasedFloat;

	// 공격자의 Damage 속성을 캡처하기 위한 정의 생성
	FGameplayEffectAttributeCaptureDefinition DamageDef(UAGAttributeSet::GetBaseDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);

	// BackingAttribute에 캡처 정의 설정
	AttributeBasedFloat.BackingAttribute = DamageDef;

	// Coefficient 설정 (음수로 설정하여 피해량 적용)
	AttributeBasedFloat.Coefficient = -1.f;

	// AttributeBasedFloat를 사용하여 ModifierMagnitude 생성
	FGameplayEffectModifierMagnitude DamageMagnitude(AttributeBasedFloat);

	// Modifier에 Magnitude 설정
	DamageModifierInfo.ModifierMagnitude = DamageMagnitude;

	// Modifier를 이펙트에 추가
	Modifiers.Add(DamageModifierInfo);
}
