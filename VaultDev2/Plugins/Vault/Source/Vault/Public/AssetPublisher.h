// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <GameFramework/Actor.h>
#include "VaultTypes.h"
#include "AssetPublisher.generated.h"


/**
 * 
 */
UCLASS()
class VAULT_API UAssetPublisher : public UObject
{
public:

	UAssetPublisher();

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Vault")
	FName Author;

	UPROPERTY(EditAnywhere, Category = "Vault")
	FName PackName;

	UPROPERTY(EditAnywhere, Category = "Vault")
	FString Description;


	UPROPERTY(EditAnywhere, Category = "Vault")
	TSoftObjectPtr<UObject> PrimaryAsset;

	UPROPERTY(VisibleAnywhere, Category = "Package List")
	FString SecondaryAssetsToInclude;




	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;

	// Package Step, Called from UI

	static bool PackageSelected(TSet<FString> PackageObjects, FVaultMetadata& Meta);

private:

	static void UpdateSystemMeta(FVaultMetadata& Metadata);
	
};


UCLASS()
class VAULT_API UAssetPublisherTags : public UObject
{
	GENERATED_BODY()
public:

	// This class for now just holds the custom details and nothing else, we might be able to 
	// scrap this and use the assetPublisher ( somehow?!)

	//UPROPERTY()
	//static FString TagsListInternal;

	//static void UpdateTagsFromGUI(FString RawTagList) { TagsListInternal = RawTagList; }


};
