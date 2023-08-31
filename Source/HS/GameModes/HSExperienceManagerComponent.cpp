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
//#include "HSLogChannels.h"

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
	DOREPLIFETIME(UHSExperienceManagerComponent, CurrentExperience);
}

void UHSExperienceManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//// deactivate any features this experience loaded
	////@TODO: This should be handled FILO as well
	//for (const FString& PluginURL : GameFeaturePluginURLs)
	//{
	//	if (ULyraExperienceManager::RequestToDeactivatePlugin(PluginURL))
	//	{
	//		UGameFeaturesSubsystem::Get().DeactivateGameFeaturePlugin(PluginURL);
	//	}
	//}

	////@TODO: Ensure proper handling of a partially-loaded state too
	//if (LoadState == ELyraExperienceLoadState::Loaded)
	//{
	//	LoadState = ELyraExperienceLoadState::Deactivating;

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
	//	for (const TObjectPtr<ULyraExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
	//	{
	//		if (ActionSet != nullptr)
	//		{
	//			DeactivateListOfActions(ActionSet->Actions);
	//		}
	//	}

	//	NumExpectedPausers = Context.GetNumPausers();

	//	if (NumExpectedPausers > 0)
	//	{
	//		UE_LOG(LogLyraExperience, Error, TEXT("Actions that have asynchronous deactivation aren't fully supported yet in Lyra experiences"));
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
}

void UHSExperienceManagerComponent::CallOrRegister_OnExperienceLoaded_HighPriority(FOnHSExperienceLoaded::FDelegate&& Delegate)
{
}

void UHSExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnHSExperienceLoaded::FDelegate&& Delegate)
{
}

void UHSExperienceManagerComponent::CallOrRegister_OnExperienceLoaded_LowPriority(FOnHSExperienceLoaded::FDelegate&& Delegate)
{
}

const UHSExperienceDefinition* UHSExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	return nullptr;
}

bool UHSExperienceManagerComponent::IsExperienceLoaded() const
{
	return false;
}

void UHSExperienceManagerComponent::OnRep_CurrentExperience()
{
}

void UHSExperienceManagerComponent::StartExperienceLoad()
{
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
