// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystem/HSAbilitySet.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "HSEquipmentManagerComponent.generated.h"

class UActorComponent;
class UHSAbilitySystemComponent;
class UHSEquipmentDefinition;
class UHSEquipmentInstance;
class UHSEquipmentManagerComponent;
class UObject;
struct FFrame;
struct FHSEquipmentList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FHSAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FHSAppliedEquipmentEntry()
	{}

	FString GetDebugString() const;

private:
	friend FHSEquipmentList;
	friend UHSEquipmentManagerComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<UHSEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UHSEquipmentInstance> Instance = nullptr;

	// Authority-only list of granted handles
	UPROPERTY(NotReplicated)
	FHSAbilitySet_GrantedHandles GrantedHandles;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FHSEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FHSEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FHSEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FHSAppliedEquipmentEntry, FHSEquipmentList>(Entries, DeltaParms, *this);
	}

	UHSEquipmentInstance* AddEntry(TSubclassOf<UHSEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UHSEquipmentInstance* Instance);

private:
	UHSAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UHSEquipmentManagerComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FHSAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FHSEquipmentList> : public TStructOpsTypeTraitsBase2<FHSEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};










/**
 * Manages equipment applied to a pawn
 */
UCLASS(BlueprintType, Const)
class HS_API UHSEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UHSEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UHSEquipmentInstance* EquipItem(TSubclassOf<UHSEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UHSEquipmentInstance* ItemInstance);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UHSEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UHSEquipmentInstance> InstanceType);

 	/** Returns all equipped instances of a given type, or an empty array if none are found */
 	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UHSEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UHSEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FHSEquipmentList EquipmentList;
};
