// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "HSInventoryManagerComponent.generated.h"

class UHSInventoryItemDefinition;
class UHSInventoryItemInstance;
class UHSInventoryManagerComponent;
class UObject;
struct FFrame;
struct FHSInventoryList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/** A message when an item is added to the inventory */
USTRUCT(BlueprintType)
struct FHSInventoryChangeMessage
{
	GENERATED_BODY()

	//@TODO: Tag based names+owning actors for inventories instead of directly exposing the component?
	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UHSInventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 Delta = 0;
};

/** A single entry in an inventory */
USTRUCT(BlueprintType)
struct FHSInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FHSInventoryEntry()
	{}

	FString GetDebugString() const;

private:
	friend FHSInventoryList;
	friend UHSInventoryManagerComponent;

	UPROPERTY()
	TObjectPtr<UHSInventoryItemInstance> Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

/** List of inventory items */
USTRUCT(BlueprintType)
struct FHSInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FHSInventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FHSInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	TArray<UHSInventoryItemInstance*> GetAllItems() const;

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FHSInventoryEntry, FHSInventoryList>(Entries, DeltaParms, *this);
	}

	UHSInventoryItemInstance* AddEntry(TSubclassOf<UHSInventoryItemDefinition> ItemClass, int32 StackCount);
	void AddEntry(UHSInventoryItemInstance* Instance);

	void RemoveEntry(UHSInventoryItemInstance* Instance);

private:
	void BroadcastChangeMessage(FHSInventoryEntry& Entry, int32 OldCount, int32 NewCount);

private:
	friend UHSInventoryManagerComponent;

private:
	// Replicated list of items
	UPROPERTY()
	TArray<FHSInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FHSInventoryList> : public TStructOpsTypeTraitsBase2<FHSInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};










/**
 * Manages an inventory
 */
UCLASS(BlueprintType)
class HS_API UHSInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHSInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	bool CanAddItemDefinition(TSubclassOf<UHSInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UHSInventoryItemInstance* AddItemDefinition(TSubclassOf<UHSInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddItemInstance(UHSInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveItemInstance(UHSInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	TArray<UHSInventoryItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	UHSInventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UHSInventoryItemDefinition> ItemDef) const;

	int32 GetTotalItemCountByDefinition(TSubclassOf<UHSInventoryItemDefinition> ItemDef) const;
	bool ConsumeItemsByDefinition(TSubclassOf<UHSInventoryItemDefinition> ItemDef, int32 NumToConsume);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UObject interface

private:
	UPROPERTY(Replicated)
	FHSInventoryList InventoryList;
};
