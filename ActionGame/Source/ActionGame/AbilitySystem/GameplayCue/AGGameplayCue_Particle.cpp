// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameplayCue_Particle.h"

bool UAGGameplayCue_Particle::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	return Super::OnActive_Implementation(MyTarget, Parameters);
	
}

bool UAGGameplayCue_Particle::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	return Super::OnExecute_Implementation(MyTarget, Parameters);
	
}

bool UAGGameplayCue_Particle::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	return Super::OnRemove_Implementation(MyTarget, Parameters);
	
}
