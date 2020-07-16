// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetPublisher.h"
#include "PakFileUtilities.h"
#include "VaultSettings.h"
#include "Misc/FileHelper.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "MetadataOps.h"
#include "Vault.h"


UAssetPublisher::UAssetPublisher()
{
	Author = FPlatformProcess::UserName();
}

void UAssetPublisher::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property)
	{
		if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAssetPublisher, PrimaryAsset))
		{
		
		}
	}
}

bool UAssetPublisher::PackageSelected(TSet<FString> PackageObjects, FVaultMetadata& Meta)
{
	UpdateSystemMeta(Meta);


	bool bWipePackageList = FVaultSettings::Get().GetVaultLocalSettings()->GetBoolField(TEXT("ClearPackageListOnSuccessfulPackage"));
	// store in a better place, make name procedural, so it can be kept for archive and even analysis by loader.

	// Dated Filename
	const FString PackageListFilename = TEXT("VaultPackageList_") + Meta.LastModified.ToString() + TEXT(".txt");

	FString ListDirectory;
	bool bFoundListDirectory = FVaultSettings::Get().GetVaultLocalSettings()->TryGetStringField(TEXT("PackageListStoragePath"), ListDirectory);

	if (!bFoundListDirectory || !FPaths::DirectoryExists(ListDirectory))
	{
		FString TempPath = FGenericPlatformMisc::GetEnvironmentVariable(TEXT("TEMP"));
		FPaths::NormalizeDirectoryName(TempPath);
		ListDirectory = TempPath;
		UE_LOG(LogVault, Error, TEXT("Unable to use PackageListStoragePath, storing file instead to : %s"), *TempPath);
	}

	FString TextDocFull = ListDirectory / PackageListFilename;
	FPaths::NormalizeDirectoryName(TextDocFull);
	UE_LOG(LogVault, Display, TEXT("Writing File List: %s"), *TextDocFull);
	
	FFileHelper::SaveStringArrayToFile(PackageObjects.Array(), *TextDocFull);
	
	const FString Root = FVaultSettings::Get().GetAssetLibraryRoot();
	const FString Filename = Meta.PackName.ToString() + TEXT(".upack");

	// Convert String to parsable command
	const FString Command = FString::Printf(TEXT("%s -create=%s"), *(Root / Filename), *TextDocFull);

	UE_LOG(LogVault, Display, TEXT("Running Pack Command: %s"), *Command);
	bool bRanPak = ExecuteUnrealPak(*Command);

	if (!bRanPak)
	{
		return false;
	}

	// Metadata Writing

	FMetadataOps::WriteMetadata(Meta);

	return true;

}

void UAssetPublisher::UpdateSystemMeta(FVaultMetadata& Metadata)
{
	Metadata.MachineID = FGenericPlatformMisc::GetLoginId();
}
