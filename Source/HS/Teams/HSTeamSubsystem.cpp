// Copyright Epic Games, Inc. All Rights Reserved.

#include "Teams/HSTeamSubsystem.h"

#include "AbilitySystemGlobals.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "HSLogChannels.h"
#include "HSTeamAgentInterface.h"
#include "HSTeamCheats.h"
#include "HSTeamPrivateInfo.h"
#include "HSTeamPublicInfo.h"
#include "Player/HSPlayerState.h"
#include "Teams/HSTeamInfoBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSTeamSubsystem)

class FSubsystemCollectionBase;

//////////////////////////////////////////////////////////////////////
// FHSTeamTrackingInfo

void FHSTeamTrackingInfo::SetTeamInfo(AHSTeamInfoBase* Info)
{
	if (AHSTeamPublicInfo* NewPublicInfo = Cast<AHSTeamPublicInfo>(Info))
	{
		ensure((PublicInfo == nullptr) || (PublicInfo == NewPublicInfo));
		PublicInfo = NewPublicInfo;

		UHSTeamDisplayAsset* OldDisplayAsset = DisplayAsset;
		DisplayAsset = NewPublicInfo->GetTeamDisplayAsset();

		if (OldDisplayAsset != DisplayAsset)
		{
			OnTeamDisplayAssetChanged.Broadcast(DisplayAsset);
		}
	}
	else if (AHSTeamPrivateInfo* NewPrivateInfo = Cast<AHSTeamPrivateInfo>(Info))
	{
		ensure((PrivateInfo == nullptr) || (PrivateInfo == NewPrivateInfo));
		PrivateInfo = NewPrivateInfo;
	}
	else
	{
		checkf(false, TEXT("Expected a public or private team info but got %s"), *GetPathNameSafe(Info))
	}
}

void FHSTeamTrackingInfo::RemoveTeamInfo(AHSTeamInfoBase* Info)
{
	if (PublicInfo == Info)
	{
		PublicInfo = nullptr;
	}
	else if (PrivateInfo == Info)
	{
		PrivateInfo = nullptr;
	}
	else
	{
		ensureMsgf(false, TEXT("Expected a previously registered team info but got %s"), *GetPathNameSafe(Info));
	}
}

//////////////////////////////////////////////////////////////////////
// UHSTeamSubsystem

UHSTeamSubsystem::UHSTeamSubsystem()
{
}

void UHSTeamSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	auto AddTeamCheats = [](UCheatManager* CheatManager)
		{
			CheatManager->AddCheatManagerExtension(NewObject<UHSTeamCheats>(CheatManager));
		};

	CheatManagerRegistrationHandle = UCheatManager::RegisterForOnCheatManagerCreated(FOnCheatManagerCreated::FDelegate::CreateLambda(AddTeamCheats));
}

void UHSTeamSubsystem::Deinitialize()
{
	UCheatManager::UnregisterFromOnCheatManagerCreated(CheatManagerRegistrationHandle);

	Super::Deinitialize();
}

bool UHSTeamSubsystem::RegisterTeamInfo(AHSTeamInfoBase* TeamInfo)
{
	if (!ensure(TeamInfo))
	{
		return false;
	}

	const int32 TeamId = TeamInfo->GetTeamId();
	if (ensure(TeamId != INDEX_NONE))
	{
		FHSTeamTrackingInfo& Entry = TeamMap.FindOrAdd(TeamId);
		Entry.SetTeamInfo(TeamInfo);

		return true;
	}

	return false;
}

bool UHSTeamSubsystem::UnregisterTeamInfo(AHSTeamInfoBase* TeamInfo)
{
	if (!ensure(TeamInfo))
	{
		return false;
	}

	const int32 TeamId = TeamInfo->GetTeamId();
	if (ensure(TeamId != INDEX_NONE))
	{
		FHSTeamTrackingInfo* Entry = TeamMap.Find(TeamId);

		// If it couldn't find the entry, this is probably a leftover actor from a previous world, ignore it
		if (Entry)
		{
			Entry->RemoveTeamInfo(TeamInfo);

			return true;
		}
	}

	return false;
}

bool UHSTeamSubsystem::ChangeTeamForActor(AActor* ActorToChange, int32 NewTeamIndex)
{
	const FGenericTeamId NewTeamID = IntegerToGenericTeamId(NewTeamIndex);
	if (AHSPlayerState* HSPS = const_cast<AHSPlayerState*>(FindPlayerStateFromActor(ActorToChange)))
	{
		HSPS->SetGenericTeamId(NewTeamID);
		return true;
	}
	else if (IHSTeamAgentInterface* TeamActor = Cast<IHSTeamAgentInterface>(ActorToChange))
	{
		TeamActor->SetGenericTeamId(NewTeamID);
		return true;
	}
	else
	{
		return false;
	}
}

int32 UHSTeamSubsystem::FindTeamFromObject(const UObject* TestObject) const
{
	// See if it's directly a team agent
	if (const IHSTeamAgentInterface* ObjectWithTeamInterface = Cast<IHSTeamAgentInterface>(TestObject))
	{
		return GenericTeamIdToInteger(ObjectWithTeamInterface->GetGenericTeamId());
	}

	if (const AActor* TestActor = Cast<const AActor>(TestObject))
	{
		// See if the instigator is a team actor
		if (const IHSTeamAgentInterface* InstigatorWithTeamInterface = Cast<IHSTeamAgentInterface>(TestActor->GetInstigator()))
		{
			return GenericTeamIdToInteger(InstigatorWithTeamInterface->GetGenericTeamId());
		}

		// TeamInfo actors don't actually have the team interface, so they need a special case
		if (const AHSTeamInfoBase* TeamInfo = Cast<AHSTeamInfoBase>(TestActor))
		{
			return TeamInfo->GetTeamId();
		}

		// Fall back to finding the associated player state
		if (const AHSPlayerState* HSPS = FindPlayerStateFromActor(TestActor))
		{
			return HSPS->GetTeamId();
		}
	}

	return INDEX_NONE;
}

const AHSPlayerState* UHSTeamSubsystem::FindPlayerStateFromActor(const AActor* PossibleTeamActor) const
{
	if (PossibleTeamActor != nullptr)
	{
		if (const APawn* Pawn = Cast<const APawn>(PossibleTeamActor))
		{
			//@TODO: Consider an interface instead or have team actors register with the subsystem and have it maintain a map? (or LWC style)
			if (AHSPlayerState* HSPS = Pawn->GetPlayerState<AHSPlayerState>())
			{
				return HSPS;
			}
		}
		else if (const AController* PC = Cast<const AController>(PossibleTeamActor))
		{
			if (AHSPlayerState* HSPS = Cast<AHSPlayerState>(PC->PlayerState))
			{
				return HSPS;
			}
		}
		else if (const AHSPlayerState* HSPS = Cast<const AHSPlayerState>(PossibleTeamActor))
		{
			return HSPS;
		}

		// Try the instigator
// 		if (AActor* Instigator = PossibleTeamActor->GetInstigator())
// 		{
// 			if (ensure(Instigator != PossibleTeamActor))
// 			{
// 				return FindPlayerStateFromActor(Instigator);
// 			}
// 		}
	}

	return nullptr;
}

EHSTeamComparison UHSTeamSubsystem::CompareTeams(const UObject* A, const UObject* B, int32& TeamIdA, int32& TeamIdB) const
{
	TeamIdA = FindTeamFromObject(Cast<const AActor>(A));
	TeamIdB = FindTeamFromObject(Cast<const AActor>(B));

	if ((TeamIdA == INDEX_NONE) || (TeamIdB == INDEX_NONE))
	{
		return EHSTeamComparison::InvalidArgument;
	}
	else
	{
		return (TeamIdA == TeamIdB) ? EHSTeamComparison::OnSameTeam : EHSTeamComparison::DifferentTeams;
	}
}

EHSTeamComparison UHSTeamSubsystem::CompareTeams(const UObject* A, const UObject* B) const
{
	int32 TeamIdA;
	int32 TeamIdB;
	return CompareTeams(A, B, /*out*/ TeamIdA, /*out*/ TeamIdB);
}

void UHSTeamSubsystem::FindTeamFromActor(const UObject* TestObject, bool& bIsPartOfTeam, int32& TeamId) const
{
	TeamId = FindTeamFromObject(TestObject);
	bIsPartOfTeam = TeamId != INDEX_NONE;
}

void UHSTeamSubsystem::AddTeamTagStack(int32 TeamId, FGameplayTag Tag, int32 StackCount)
{
	auto FailureHandler = [&](const FString& ErrorMessage)
		{
			UE_LOG(LogHSTeams, Error, TEXT("AddTeamTagStack(TeamId: %d, Tag: %s, StackCount: %d) %s"), TeamId, *Tag.ToString(), StackCount, *ErrorMessage);
		};

	if (FHSTeamTrackingInfo* Entry = TeamMap.Find(TeamId))
	{
		if (Entry->PublicInfo)
		{
			if (Entry->PublicInfo->HasAuthority())
			{
				Entry->PublicInfo->TeamTags.AddStack(Tag, StackCount);
			}
			else
			{
				FailureHandler(TEXT("failed because it was called on a client"));
			}
		}
		else
		{
			FailureHandler(TEXT("failed because there is no team info spawned yet (called too early, before the experience was ready)"));
		}
	}
	else
	{
		FailureHandler(TEXT("failed because it was passed an unknown team id"));
	}
}

void UHSTeamSubsystem::RemoveTeamTagStack(int32 TeamId, FGameplayTag Tag, int32 StackCount)
{
	auto FailureHandler = [&](const FString& ErrorMessage)
		{
			UE_LOG(LogHSTeams, Error, TEXT("RemoveTeamTagStack(TeamId: %d, Tag: %s, StackCount: %d) %s"), TeamId, *Tag.ToString(), StackCount, *ErrorMessage);
		};

	if (FHSTeamTrackingInfo* Entry = TeamMap.Find(TeamId))
	{
		if (Entry->PublicInfo)
		{
			if (Entry->PublicInfo->HasAuthority())
			{
				Entry->PublicInfo->TeamTags.RemoveStack(Tag, StackCount);
			}
			else
			{
				FailureHandler(TEXT("failed because it was called on a client"));
			}
		}
		else
		{
			FailureHandler(TEXT("failed because there is no team info spawned yet (called too early, before the experience was ready)"));
		}
	}
	else
	{
		FailureHandler(TEXT("failed because it was passed an unknown team id"));
	}
}

int32 UHSTeamSubsystem::GetTeamTagStackCount(int32 TeamId, FGameplayTag Tag) const
{
	if (const FHSTeamTrackingInfo* Entry = TeamMap.Find(TeamId))
	{
		const int32 PublicStackCount = (Entry->PublicInfo != nullptr) ? Entry->PublicInfo->TeamTags.GetStackCount(Tag) : 0;
		const int32 PrivateStackCount = (Entry->PrivateInfo != nullptr) ? Entry->PrivateInfo->TeamTags.GetStackCount(Tag) : 0;
		return PublicStackCount + PrivateStackCount;
	}
	else
	{
		UE_LOG(LogHSTeams, Verbose, TEXT("GetTeamTagStackCount(TeamId: %d, Tag: %s) failed because it was passed an unknown team id"), TeamId, *Tag.ToString());
		return 0;
	}
}

bool UHSTeamSubsystem::TeamHasTag(int32 TeamId, FGameplayTag Tag) const
{
	return GetTeamTagStackCount(TeamId, Tag) > 0;
}

bool UHSTeamSubsystem::DoesTeamExist(int32 TeamId) const
{
	return TeamMap.Contains(TeamId);
}

TArray<int32> UHSTeamSubsystem::GetTeamIDs() const
{
	TArray<int32> Result;
	TeamMap.GenerateKeyArray(Result);
	Result.Sort();
	return Result;
}

bool UHSTeamSubsystem::CanCauseDamage(const UObject* Instigator, const UObject* Target, bool bAllowDamageToSelf) const
{
	if (bAllowDamageToSelf)
	{
		if ((Instigator == Target) || (FindPlayerStateFromActor(Cast<AActor>(Instigator)) == FindPlayerStateFromActor(Cast<AActor>(Target))))
		{
			return true;
		}
	}

	int32 InstigatorTeamId;
	int32 TargetTeamId;
	const EHSTeamComparison Relationship = CompareTeams(Instigator, Target, /*out*/ InstigatorTeamId, /*out*/ TargetTeamId);
	if (Relationship == EHSTeamComparison::DifferentTeams)
	{
		return true;
	}
	else if ((Relationship == EHSTeamComparison::InvalidArgument) && (InstigatorTeamId != INDEX_NONE))
	{
		// Allow damaging non-team actors for now, as long as they have an ability system component
		//@TODO: This is temporary until the target practice dummy has a team assignment
		return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Cast<const AActor>(Target)) != nullptr;
	}

	return false;
}

UHSTeamDisplayAsset* UHSTeamSubsystem::GetTeamDisplayAsset(int32 TeamId, int32 ViewerTeamId)
{
	// Currently ignoring ViewerTeamId

	if (FHSTeamTrackingInfo* Entry = TeamMap.Find(TeamId))
	{
		return Entry->DisplayAsset;
	}

	return nullptr;
}

UHSTeamDisplayAsset* UHSTeamSubsystem::GetEffectiveTeamDisplayAsset(int32 TeamId, UObject* ViewerTeamAgent)
{
	return GetTeamDisplayAsset(TeamId, FindTeamFromObject(ViewerTeamAgent));
}

void UHSTeamSubsystem::NotifyTeamDisplayAssetModified(UHSTeamDisplayAsset* /*ModifiedAsset*/)
{
	// Broadcasting to all observers when a display asset is edited right now, instead of only the edited one
	for (const auto& KVP : TeamMap)
	{
		const int32 TeamId = KVP.Key;
		const FHSTeamTrackingInfo& TrackingInfo = KVP.Value;

		TrackingInfo.OnTeamDisplayAssetChanged.Broadcast(TrackingInfo.DisplayAsset);
	}
}

FOnHSTeamDisplayAssetChangedDelegate& UHSTeamSubsystem::GetTeamDisplayAssetChangedDelegate(int32 TeamId)
{
	return TeamMap.FindOrAdd(TeamId).OnTeamDisplayAssetChanged;
}

