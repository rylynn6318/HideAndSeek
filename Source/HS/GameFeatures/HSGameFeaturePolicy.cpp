#include "GameFeatures/HSGameFeaturePolicy.h"
#include "AbilitySystem/HSGameplayCueManager.h"
#include "GameFeatureData.h"
#include "GameplayCueSet.h"

UHSGameFeaturePolicy::UHSGameFeaturePolicy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UHSGameFeaturePolicy& UHSGameFeaturePolicy::Get()
{
	return UGameFeaturesSubsystem::Get().GetPolicy<UHSGameFeaturePolicy>();
}

void UHSGameFeaturePolicy::InitGameFeatureManager()
{
	//Observers.Add(NewObject<UHSGameFeature_HotfixManager>());
	//Observers.Add(NewObject<UHSGameFeature_AddGameplayCuePaths>());

	UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		Subsystem.AddObserver(Observer);
	}

	Super::InitGameFeatureManager();
}

void UHSGameFeaturePolicy::ShutdownGameFeatureManager()
{
	Super::ShutdownGameFeatureManager();

	UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		Subsystem.RemoveObserver(Observer);
	}
	Observers.Empty();
}

TArray<FPrimaryAssetId> UHSGameFeaturePolicy::GetPreloadAssetListForGameFeature(const UGameFeatureData* GameFeatureToLoad, bool bIncludeLoadedAssets) const
{
	return Super::GetPreloadAssetListForGameFeature(GameFeatureToLoad, bIncludeLoadedAssets);
}

const TArray<FName> UHSGameFeaturePolicy::GetPreloadBundleStateForGameFeature() const
{
	return Super::GetPreloadBundleStateForGameFeature();
}

void UHSGameFeaturePolicy::GetGameFeatureLoadingMode(bool& bLoadClientData, bool& bLoadServerData) const
{
	// Editor will load both, this can cause hitching as the bundles are set to not preload in editor
	bLoadClientData = !IsRunningDedicatedServer();
	bLoadServerData = !IsRunningClientOnly();
}

bool UHSGameFeaturePolicy::IsPluginAllowed(const FString& PluginURL) const
{
	return Super::IsPluginAllowed(PluginURL);
}

//////////////////////////////////////////////////////////////////////
//
//
//#include "Hotfix/HSHotfixManager.h"
//
//void UHSGameFeature_HotfixManager::OnGameFeatureLoading(const UGameFeatureData* GameFeatureData, const FString& PluginURL)
//{
//	if (UHSHotfixManager* HotfixManager = Cast<UHSHotfixManager>(UOnlineHotfixManager::Get(nullptr)))
//	{
//		HotfixManager->RequestPatchAssetsFromIniFiles();
//	}
//}
//
////////////////////////////////////////////////////////////////////////
////
//
//#include "AbilitySystemGlobals.h"
//#include "GameFeatureAction_AddGameplayCuePath.h"
//
//#include UE_INLINE_GENERATED_CPP_BY_NAME(HSGameFeaturePolicy)
//
//class FName;
//struct FPrimaryAssetId;
//
//void UHSGameFeature_AddGameplayCuePaths::OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
//{
//	TRACE_CPUPROFILER_EVENT_SCOPE(UHSGameFeature_AddGameplayCuePaths::OnGameFeatureRegistering);
//	
//	const FString PluginRootPath = TEXT("/") + PluginName;
//	for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
//	{
//		if (const UGameFeatureAction_AddGameplayCuePath* AddGameplayCueGFA = Cast<UGameFeatureAction_AddGameplayCuePath>(Action))
//		{
//			const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCueGFA->GetDirectoryPathsToAdd();
//			
//			if (UHSGameplayCueManager* GCM = UHSGameplayCueManager::Get())
//			{
//				UGameplayCueSet* RuntimeGameplayCueSet = GCM->GetRuntimeCueSet();
//				const int32 PreInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;
//
//				for (const FDirectoryPath& Directory : DirsToAdd)
//				{
//					FString MutablePath = Directory.Path;
//					UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);
//					GCM->AddGameplayCueNotifyPath(MutablePath, /** bShouldRescanCueAssets = */ false);	
//				}
//				
//				// Rebuild the runtime library with these new paths
//				if (!DirsToAdd.IsEmpty())
//				{
//					GCM->InitializeRuntimeObjectLibrary();	
//				}
//
//				const int32 PostInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;
//				if (PreInitializeNumCues != PostInitializeNumCues)
//				{
//					GCM->RefreshGameplayCuePrimaryAsset();
//				}
//			}
//		}
//	}
//}
//
//void UHSGameFeature_AddGameplayCuePaths::OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
//{
//	const FString PluginRootPath = TEXT("/") + PluginName;
//	for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
//	{
//		if (const UGameFeatureAction_AddGameplayCuePath* AddGameplayCueGFA = Cast<UGameFeatureAction_AddGameplayCuePath>(Action))
//		{
//			const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCueGFA->GetDirectoryPathsToAdd();
//			
//			if (UGameplayCueManager* GCM = UAbilitySystemGlobals::Get().GetGameplayCueManager())
//			{
//				int32 NumRemoved = 0;
//				for (const FDirectoryPath& Directory : DirsToAdd)
//				{
//					FString MutablePath = Directory.Path;
//					UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);
//					NumRemoved += GCM->RemoveGameplayCueNotifyPath(MutablePath, /** bShouldRescanCueAssets = */ false);
//				}
//
//				ensure(NumRemoved == DirsToAdd.Num());
//				
//				// Rebuild the runtime library only if there is a need to
//				if (NumRemoved > 0)
//				{
//					GCM->InitializeRuntimeObjectLibrary();	
//				}			
//			}
//	}
//	}
//}
