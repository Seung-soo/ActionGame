// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AGTableData.generated.h"

USTRUCT()
struct FAGDataTableStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DataTableTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> DataTable = nullptr;
};

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGTableData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	const UDataTable* FindDataTableByTag(const FGameplayTag& DataTableTag) const;

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAGDataTableStruct> DataTables;
};
