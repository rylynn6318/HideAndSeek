#include "System/HSAssetManager.h"
#include "AbilitySystem/HSAbilitySystemGlobals.h"
#include "Character/HSPawnData.h"
#include "Engine/Engine.h"
#include "Misc/ScopedSlowTask.h"
#include "Stats/StatsMisc.h"
#include "Misc/App.h"
#include "HSLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSAssetManager)

const FName FHSBundles::Equipped("Equipped");

UHSAssetManager& UHSAssetManager::Get()
{
	check(GEngine);

	if (UHSAssetManager* Singleton = Cast<UHSAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogHS, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to HSAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UHSAssetManager>();
}

void UHSAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UHSAbilitySystemGlobals::Get().InitGlobalData();
}

const UHSPawnData* UHSAssetManager::GetDefaultPawnData() const
{
	return GetAsset(DefaultPawnData);
}

void UHSAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}

UObject* UHSAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

bool UHSAssetManager::ShouldLogAssetLoads()
{
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}