

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "HSAssetManager.generated.h"

class UHSPawnData;

struct FHSBundles
{
	static const FName Equipped;
};

UCLASS(Config = Game)
class HS_API UHSAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	// Returns the AssetManager singleton object.
	static UHSAssetManager& Get();

	// Returns the asset referenced by a TSoftObjectPtr.  This will synchronously load the asset if it's not already loaded.
	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);


	virtual void StartInitialLoading() override;
	
	const UHSPawnData* GetDefaultPawnData() const;

protected:
	// Thread safe way of adding a loaded asset to keep in memory.
	void AddLoadedAsset(const UObject* Asset);

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
	static bool ShouldLogAssetLoads();

protected:
	// Pawn data used when spawning player pawns if there isn't one set on the player state.
	UPROPERTY(Config)
	TSoftObjectPtr<UHSPawnData> DefaultPawnData;

private:
	// Assets loaded and tracked by the asset manager.
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	// Used for a scope lock when modifying the list of load assets.
	FCriticalSection LoadedAssetsCritical;
};

template<typename AssetType>
AssetType* UHSAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}
