// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "HSInventoryItemDefinition.generated.h"

template <typename T> class TSubclassOf;

class UHSInventoryItemInstance;
struct FFrame;

//////////////////////////////////////////////////////////////////////

// Represents a fragment of an item definition
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class HS_API UHSInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UHSInventoryItemInstance* Instance) const {}
};

//////////////////////////////////////////////////////////////////////

/**
 * UHSInventoryItemDefinition
 */
UCLASS(Blueprintable, Const, Abstract)
class UHSInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UHSInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UHSInventoryItemFragment>> Fragments;

public:
	const UHSInventoryItemFragment* FindFragmentByClass(TSubclassOf<UHSInventoryItemFragment> FragmentClass) const;
};

//@TODO: Make into a subsystem instead?
UCLASS()
class UHSInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UHSInventoryItemFragment* FindItemDefinitionFragment(TSubclassOf<UHSInventoryItemDefinition> ItemDef, TSubclassOf<UHSInventoryItemFragment> FragmentClass);
};
