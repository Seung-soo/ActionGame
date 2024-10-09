// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameInstance.h"
#include "AGAssetManager.h"

UAGGameInstance::UAGGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UAGGameInstance::Init()
{
	Super::Init();

	UAGAssetManager::Initialize();
}

void UAGGameInstance::Shutdown()
{
	Super::Shutdown();
}
