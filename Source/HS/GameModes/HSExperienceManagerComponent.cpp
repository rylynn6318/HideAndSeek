#include "GameModes/HSExperienceManagerComponent.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "HSExperienceDefinition.h"
#include "HSExperienceActionSet.h"
//#include "HSExperienceManager.h"
#include "GameFeaturesSubsystem.h"
#include "System/HSAssetManager.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystemSettings.h"
#include "TimerManager.h"
//#include "Settings/HSSettingsLocal.h"
#include "HSLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSExperienceManagerComponent)

//@TODO: Async load the experience definition itself
//@TODO: Handle failures explicitly (go into a 'completed but failed' state rather than check()-ing)
//@TODO: Do the action phases at the appropriate times instead of all at once
//@TODO: Support deactivating an experience and do the unloading actions
//@TODO: Think about what deactivation/cleanup means for preloaded assets
//@TODO: Handle deactivating game features, right now we 'leak' them enabled
// (for a client moving from experience to experience we actually want to diff the requirements and only unload some, not unload everything for them to just be immediately reloaded)
//@TODO: Handle both built-in and URL-based plugins (search for colon?)

UHSExperienceManagerComponent::UHSExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UHSExperienceManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHSExperienceManagerComponent, CurrentExperience);
}

void UHSExperienceManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//// deactivate any features this experience loaded
	////@TODO: This should be handled FILO as well
	//for (const FString& PluginURL : GameFeaturePluginURLs)
	//{
	//	if (UHSExperienceManager::RequestToDeactivatePlugin(PluginURL))
	//	{
	//		UGameFeaturesSubsystem::Get().DeactivateGameFeaturePlugin(PluginURL);
	//	}
	//}

	////@TODO: Ensure proper handling of a partially-loaded state too
	//if (LoadState == EHSExperienceLoadState::Loaded)
	//{
	//	LoadState = EHSExperienceLoadState::Deactivating;

	//	// Make sure we won't complete the transition prematurely if someone registers as a pauser but fires immediately
	//	NumExpectedPausers = INDEX_NONE;
	//	NumObservedPausers = 0;

	//	// Deactivate and unload the actions
	//	FGameFeatureDeactivatingContext Context(FSimpleDelegate::CreateUObject(this, &ThisClass::OnActionDeactivationCompleted));

	//	const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
	//	if (ExistingWorldContext)
	//	{
	//		Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
	//	}

	//	auto DeactivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
	//		{
	//			for (UGameFeatureAction* Action : ActionList)
	//			{
	//				if (Action)
	//				{
	//					Action->OnGameFeatureDeactivating(Context);
	//					Action->OnGameFeatureUnregistering();
	//				}
	//			}
	//		};

	//	DeactivateListOfActions(CurrentExperience->Actions);
	//	for (const TObjectPtr<UHSExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
	//	{
	//		if (ActionSet != nullptr)
	//		{
	//			DeactivateListOfActions(ActionSet->Actions);
	//		}
	//	}

	//	NumExpectedPausers = Context.GetNumPausers();

	//	if (NumExpectedPausers > 0)
	//	{
	//		UE_LOG(LogHSExperience, Error, TEXT("Actions that have asynchronous deactivation aren't fully supported yet in HS experiences"));
	//	}

	//	if (NumExpectedPausers == NumObservedPausers)
	//	{
	//		OnAllActionsDeactivated();
	//	}
	//}
}

//bool UHSExperienceManagerComponent::ShouldShowLoadingScreen(FString& OutReason) const
//{
//	if (LoadState != EHSExperienceLoadState::Loaded)
//	{
//		OutReason = TEXT("Experience still loading");
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

void UHSExperienceManagerComponent::SetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	UHSAssetManager& AssetManager = UHSAssetManager::Get();
	FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
	TSubclassOf<UHSExperienceDefinition> AssetClass = Cast<UClass>(AssetPath.TryLoad());
	check(AssetClass);
	const UHSExperienceDefinition* Experience = GetDefault<UHSExperienceDefinition>(AssetClass);

	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	CurrentExperience = Experience;
	StartExperienceLoad();
}

void UHSExperienceManagerComponent::CallOrRegister_OnExperienceLoaded_HighPriority(FOnHSExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded_HighPriority.Add(MoveTemp(Delegate));
	}
}

void UHSExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnHSExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void UHSExperienceManagerComponent::CallOrRegister_OnExperienceLoaded_LowPriority(FOnHSExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded_LowPriority.Add(MoveTemp(Delegate));
	}
}

const UHSExperienceDefinition* UHSExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == EHSExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}

bool UHSExperienceManagerComponent::IsExperienceLoaded() const
{
	return (LoadState == EHSExperienceLoadState::Loaded) && (CurrentExperience != nullptr);
}

void UHSExperienceManagerComponent::OnRep_CurrentExperience()
{
	StartExperienceLoad();
}

void UHSExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience != nullptr);
	check(LoadState == EHSExperienceLoadState::Unloaded);

	UE_LOG(LogHSExperience, Log, TEXT("EXPERIENCE: StartExperienceLoad(CurrentExperience = %s, %s)"),
		*CurrentExperience->GetPrimaryAssetId().ToString(),
		*GetClientServerContextString(this));

	LoadState = EHSExperienceLoadState::Loading;

	UHSAssetManager& AssetManager = UHSAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;
	TSet<FSoftObjectPath> RawAssetList;

	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());
	for (const TObjectPtr<UHSExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
	{
		if (ActionSet != nullptr)
		{
			BundleAssetList.Add(ActionSet->GetPrimaryAssetId());
		}
	}

	// Load assets associated with the experience

	TArray<FName> BundlesToLoad;
	BundlesToLoad.Add(FHSBundles::Equipped);

	//@TODO: Centralize this client/server stuff into the HSAssetManager
	const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
	const bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
	const bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
	if (bLoadClient)
	{
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
	}
	if (bLoadServer)
	{
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
	}

	TSharedPtr<FStreamableHandle> BundleLoadHandle = nullptr;
	if (BundleAssetList.Num() > 0)
	{
		BundleLoadHandle = AssetManager.ChangeBundleStateForPrimaryAssets(BundleAssetList.Array(), BundlesToLoad, {}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);
	}

	TSharedPtr<FStreamableHandle> RawLoadHandle = nullptr;
	if (RawAssetList.Num() > 0)
	{
		RawLoadHandle = AssetManager.LoadAssetList(RawAssetList.Array(), FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority, TEXT("StartExperienceLoad()"));
	}

	// If both async loads are running, combine them
	TSharedPtr<FStreamableHandle> Handle = nullptr;
	if (BundleLoadHandle.IsValid() && RawLoadHandle.IsValid())
	{
		Handle = AssetManager.GetStreamableManager().CreateCombinedHandle({ BundleLoadHandle, RawLoadHandle });
	}
	else
	{
		Handle = BundleLoadHandle.IsValid() ? BundleLoadHandle : RawLoadHandle;
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);
	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// Assets were already loaded, call the delegate now
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);

		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}

	// This set of assets gets preloaded, but we don't block the start of the experience based on it
	TSet<FPrimaryAssetId> PreloadAssetList;
	//@TODO: Determine assets to preload (but not blocking-ly)
	if (PreloadAssetList.Num() > 0)
	{
		AssetManager.ChangeBundleStateForPrimaryAssets(PreloadAssetList.Array(), BundlesToLoad, {});
	}
}

void UHSExperienceManagerComponent::OnExperienceLoadComplete()
{
}

void UHSExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
}

void UHSExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
}

void UHSExperienceManagerComponent::OnActionDeactivationCompleted()
{
}

void UHSExperienceManagerComponent::OnAllActionsDeactivated()
{
	//@TODO: We actually only deactivated and didn't fully unload...
	LoadState = EHSExperienceLoadState::Unloaded;
	CurrentExperience = nullptr;
	//@TODO:	GEngine->ForceGarbageCollection(true);
}