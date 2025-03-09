// Copyright Epic Games, Inc. All Rights Reserved.

#include "HSInventoryManagerComponent.h"

#include "Engine/ActorChannel.h"
#include "Engine/World.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "HSInventoryItemDefinition.h"
#include "HSInventoryItemInstance.h"
#include "NativeGameplayTags.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSInventoryManagerComponent)

class FLifetimeProperty;
struct FReplicationFlags;

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_HS_Inventory_Message_StackChanged, "HS.Inventory.Message.StackChanged");

//////////////////////////////////////////////////////////////////////
// FHSInventoryEntry

FString FHSInventoryEntry::GetDebugString() const
{
	TSubclassOf<UHSInventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));
}

//////////////////////////////////////////////////////////////////////
// FHSInventoryList

void FHSInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FHSInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FHSInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FHSInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FHSInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FHSInventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FHSInventoryList::BroadcastChangeMessage(FHSInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	FHSInventoryChangeMessage Message;
	Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
	MessageSystem.BroadcastMessage(TAG_HS_Inventory_Message_StackChanged, Message);
}

UHSInventoryItemInstance* FHSInventoryList::AddEntry(TSubclassOf<UHSInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UHSInventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
 	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FHSInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UHSInventoryItemInstance>(OwnerComponent->GetOwner());  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDef);
	for (UHSInventoryItemFragment* Fragment : GetDefault<UHSInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;

	//const UHSInventoryItemDefinition* ItemCDO = GetDefault<UHSInventoryItemDefinition>(ItemDef);
	MarkItemDirty(NewEntry);

	return Result;
}

void FHSInventoryList::AddEntry(UHSInventoryItemInstance* Instance)
{
	unimplemented();
}

void FHSInventoryList::RemoveEntry(UHSInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FHSInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

TArray<UHSInventoryItemInstance*> FHSInventoryList::GetAllItems() const
{
	TArray<UHSInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FHSInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

//////////////////////////////////////////////////////////////////////
// UHSInventoryManagerComponent

UHSInventoryManagerComponent::UHSInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

void UHSInventoryManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

bool UHSInventoryManagerComponent::CanAddItemDefinition(TSubclassOf<UHSInventoryItemDefinition> ItemDef, int32 StackCount)
{
	//@TODO: Add support for stack limit / uniqueness checks / etc...
	return true;
}

UHSInventoryItemInstance* UHSInventoryManagerComponent::AddItemDefinition(TSubclassOf<UHSInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UHSInventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
		
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void UHSInventoryManagerComponent::AddItemInstance(UHSInventoryItemInstance* ItemInstance)
{
	InventoryList.AddEntry(ItemInstance);
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
}

void UHSInventoryManagerComponent::RemoveItemInstance(UHSInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UHSInventoryItemInstance*> UHSInventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

UHSInventoryItemInstance* UHSInventoryManagerComponent::FindFirstItemStackByDefinition(TSubclassOf<UHSInventoryItemDefinition> ItemDef) const
{
	for (const FHSInventoryEntry& Entry : InventoryList.Entries)
	{
		UHSInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

int32 UHSInventoryManagerComponent::GetTotalItemCountByDefinition(TSubclassOf<UHSInventoryItemDefinition> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FHSInventoryEntry& Entry : InventoryList.Entries)
	{
		UHSInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				++TotalCount;
			}
		}
	}

	return TotalCount;
}

bool UHSInventoryManagerComponent::ConsumeItemsByDefinition(TSubclassOf<UHSInventoryItemDefinition> ItemDef, int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	//@TODO: N squared right now as there's no acceleration structure
	int32 TotalConsumed = 0;
	while (TotalConsumed < NumToConsume)
	{
		if (UHSInventoryItemInstance* Instance = UHSInventoryManagerComponent::FindFirstItemStackByDefinition(ItemDef))
		{
			InventoryList.RemoveEntry(Instance);
			++TotalConsumed;
		}
		else
		{
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

void UHSInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing UHSInventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FHSInventoryEntry& Entry : InventoryList.Entries)
		{
			UHSInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

bool UHSInventoryManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FHSInventoryEntry& Entry : InventoryList.Entries)
	{
		UHSInventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

//////////////////////////////////////////////////////////////////////
//

// UCLASS(Abstract)
// class UHSInventoryFilter : public UObject
// {
// public:
// 	virtual bool PassesFilter(UHSInventoryItemInstance* Instance) const { return true; }
// };

// UCLASS()
// class UHSInventoryFilter_HasTag : public UHSInventoryFilter
// {
// public:
// 	virtual bool PassesFilter(UHSInventoryItemInstance* Instance) const { return true; }
// };


