// Copyright Epic Games, Inc. All Rights Reserved.

#include "Accolades/HSAccoladeHostWidget.h"

#include "DataRegistrySubsystem.h"
#include "HSLogChannels.h"
#include "Messages/HSNotificationMessage.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSAccoladeHostWidget)

class UUserWidget;

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_HS_ShooterGame_Accolade, "HS.ShooterGame.Accolade");

static FName NAME_AccoladeRegistryID("Accolades");

void UHSAccoladeHostWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	ListenerHandle = MessageSubsystem.RegisterListener(TAG_HS_AddNotification_Message, this, &ThisClass::OnNotificationMessage);
}

void UHSAccoladeHostWidget::NativeDestruct()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(ListenerHandle);

	CancelAsyncLoading();

	Super::NativeDestruct();
}

void UHSAccoladeHostWidget::OnNotificationMessage(FGameplayTag Channel, const FHSNotificationMessage& Notification)
{
	if (Notification.TargetChannel == TAG_HS_ShooterGame_Accolade)
	{
		// Ignore notifications for other players
		if (Notification.TargetPlayer != nullptr)
		{
			APlayerController* PC = GetOwningPlayer();
			if ((PC == nullptr) || (PC->PlayerState != Notification.TargetPlayer))
			{
				return;
			}
		}

		// Load the data registry row for this accolade
		const int32 NextID = AllocatedSequenceID;
		++AllocatedSequenceID;

		FDataRegistryId ItemID(NAME_AccoladeRegistryID, Notification.PayloadTag.GetTagName());
		if (!UDataRegistrySubsystem::Get()->AcquireItem(ItemID, FDataRegistryItemAcquiredCallback::CreateUObject(this, &ThisClass::OnRegistryLoadCompleted, NextID)))
		{
			UE_LOG(LogHS, Error, TEXT("Failed to find accolade registry for tag %s, accolades will not appear"), *Notification.PayloadTag.GetTagName().ToString());
			--AllocatedSequenceID;
		}
	}
}

void UHSAccoladeHostWidget::OnRegistryLoadCompleted(const FDataRegistryAcquireResult& AccoladeHandle, int32 SequenceID)
{
	if (const FHSAccoladeDefinitionRow* AccoladeRow = AccoladeHandle.GetItem<FHSAccoladeDefinitionRow>())
	{
		FPendingAccoladeEntry& PendingEntry = PendingAccoladeLoads.AddDefaulted_GetRef();
		PendingEntry.Row = *AccoladeRow;
		PendingEntry.SequenceID = SequenceID;

		TArray<FSoftObjectPath> AssetsToLoad;
		AssetsToLoad.Add(AccoladeRow->Sound.ToSoftObjectPath());
		AssetsToLoad.Add(AccoladeRow->Icon.ToSoftObjectPath());
		AsyncLoad(AssetsToLoad, [this, SequenceID]
		{
			FPendingAccoladeEntry* EntryThatFinishedLoading = PendingAccoladeLoads.FindByPredicate([SequenceID](const FPendingAccoladeEntry& Entry) { return Entry.SequenceID == SequenceID; });
			if (ensure(EntryThatFinishedLoading))
			{
				EntryThatFinishedLoading->Sound = EntryThatFinishedLoading->Row.Sound.Get();
				EntryThatFinishedLoading->Icon = EntryThatFinishedLoading->Row.Icon.Get();
				EntryThatFinishedLoading->bFinishedLoading = true;
				ConsiderLoadedAccolades();
			}
		});
		StartAsyncLoading();
	}
	else
	{
		ensure(false);
	}
}

void UHSAccoladeHostWidget::ConsiderLoadedAccolades()
{
	int32 PendingIndexToDisplay;
	do
	{
		PendingIndexToDisplay = PendingAccoladeLoads.IndexOfByPredicate([DesiredID=NextDisplaySequenceID](const FPendingAccoladeEntry& Entry) { return Entry.bFinishedLoading && Entry.SequenceID == DesiredID; });
		if (PendingIndexToDisplay != INDEX_NONE)
		{
			FPendingAccoladeEntry Entry = MoveTemp(PendingAccoladeLoads[PendingIndexToDisplay]);
			PendingAccoladeLoads.RemoveAtSwap(PendingIndexToDisplay);

			ProcessLoadedAccolade(Entry);
			++NextDisplaySequenceID;
		}
	} while (PendingIndexToDisplay != INDEX_NONE);
}

void UHSAccoladeHostWidget::ProcessLoadedAccolade(const FPendingAccoladeEntry& Entry)
{
	if (Entry.Row.LocationTag == LocationName)
	{
		bool bRecreateWidget = PendingAccoladeDisplays.Num() == 0;
		for (int32 Index = 0; Index < PendingAccoladeDisplays.Num(); )
		{
			if (PendingAccoladeDisplays[Index].Row.AccoladeTags.HasAny(Entry.Row.CancelAccoladesWithTag))
			{
				if (UUserWidget* OldWidget = PendingAccoladeDisplays[Index].AllocatedWidget)
				{
					DestroyAccoladeWidget(OldWidget);
					bRecreateWidget = true;
				}
				PendingAccoladeDisplays.RemoveAt(Index);
			}
			else
			{
				++Index;
			}
		}

		PendingAccoladeDisplays.Add(Entry);
		
		if (bRecreateWidget)
		{
			DisplayNextAccolade();
		}
	}
}

void UHSAccoladeHostWidget::DisplayNextAccolade()
{
	if (PendingAccoladeDisplays.Num() > 0)
	{
		FPendingAccoladeEntry& Entry = PendingAccoladeDisplays[0];

		GetWorld()->GetTimerManager().SetTimer(NextTimeToReconsiderHandle, this, &ThisClass::PopDisplayedAccolade, Entry.Row.DisplayDuration);
		Entry.AllocatedWidget = CreateAccoladeWidget(Entry);
	}
}

void UHSAccoladeHostWidget::PopDisplayedAccolade()
{
	if (PendingAccoladeDisplays.Num() > 0)
	{
		if (UUserWidget* OldWidget = PendingAccoladeDisplays[0].AllocatedWidget)
		{
			DestroyAccoladeWidget(OldWidget);
		}
		PendingAccoladeDisplays.RemoveAt(0);
	}

	DisplayNextAccolade();
}

