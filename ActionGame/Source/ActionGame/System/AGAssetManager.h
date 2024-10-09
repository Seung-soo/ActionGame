// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "../Data/AGAssetData.h"
#include "AGAssetManager.generated.h"

DECLARE_DELEGATE_TwoParams(FAsyncLoadCompletedDelegate, const FName&/*AssetName or Label*/, UObject*/*LoadedAsset*/);

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAGAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	UAGAssetManager();
    
	static UAGAssetManager& Get();

public:
	static void Initialize();

	template<typename AssetType>
	static AssetType* GetAssetByName(const FName& AssetName);

	static void LoadSyncByPath(const FSoftObjectPath& AssetPath);
	static void LoadSyncByName(const FName& AssetName);
	static void LoadSyncByLabel(const FName& Label);

	static void LoadAsyncByPath(const FSoftObjectPath& AssetPath, FAsyncLoadCompletedDelegate CompletedDelegate = FAsyncLoadCompletedDelegate());
	static void LoadAsyncByName(const FName& AssetName, FAsyncLoadCompletedDelegate CompletedDelegate = FAsyncLoadCompletedDelegate());

	static void ReleaseSyncByPath(const FSoftObjectPath& AssetPath);
	static void ReleaseSyncByName(const FName& AssetName);
	static void ReleaseSyncByLabel(const FName& Label);
	static void ReleaseAll();

private:
	void LoadPreloadAssets();
	void AddLoadedAsset(const FName& AssetName, const UObject* Asset);

private:
	UPROPERTY()
	TObjectPtr<UAGAssetData> LoadedAssetData;

	UPROPERTY()
	TMap<FName, TObjectPtr<const UObject>> NameToLoadedAsset;
};


 template <typename AssetType>
 AssetType* UAGAssetManager::GetAssetByName(const FName& AssetName)
 {
 	UAGAssetData* AssetData = Get().LoadedAssetData;
 	check(AssetData);

 	AssetType* LoadedAsset = nullptr;
 	const FSoftObjectPath& AssetPath = AssetData->GetAssetPathByName(AssetName);
 	if (AssetPath.IsValid())
 	{
 		LoadedAsset = Cast<AssetType>(AssetPath.ResolveObject());
 		if (nullptr == LoadedAsset)
 		{
 			UE_LOG(LogTemp, Warning, TEXT("Attempted sync loading because asset hadn't loaded yet [%s]."), *AssetPath.ToString());
 			LoadedAsset = Cast<AssetType>(AssetPath.TryLoad());
 		}
 	}

 	return LoadedAsset;
 }
