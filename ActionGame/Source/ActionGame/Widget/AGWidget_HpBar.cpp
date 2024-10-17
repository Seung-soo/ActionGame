// Fill out your copyright notice in the Description page of Project Settings.


#include "AGWidget_HpBar.h"
#include "Components/ProgressBar.h"

void UAGWidget_HpBar::SetHpRatio(float Ratio)
{
	if (IsValid(HpBar))
	{
		HpBar->SetPercent(Ratio);
	}
}
