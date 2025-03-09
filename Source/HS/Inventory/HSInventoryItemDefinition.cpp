// Copyright Epic Games, Inc. All Rights Reserved.

#include "HSInventoryItemDefinition.h"

#include "Templates/SubclassOf.h"
#include "UObject/ObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSInventoryItemDefinition)

//////////////////////////////////////////////////////////////////////
// UHSInventoryItemDefinition

UHSInventoryItemDefinition::UHSInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UHSInventoryItemFragment* UHSInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UHSInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UHSInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////
// UHSInventoryItemDefinition

const UHSInventoryItemFragment* UHSInventoryFunctionLibrary::FindItemDefinitionFragment(TSubclassOf<UHSInventoryItemDefinition> ItemDef, TSubclassOf<UHSInventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UHSInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}

