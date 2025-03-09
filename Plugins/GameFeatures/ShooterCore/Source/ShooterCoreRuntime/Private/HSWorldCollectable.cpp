// Copyright Epic Games, Inc. All Rights Reserved.

#include "HSWorldCollectable.h"

#include "Async/TaskGraphInterfaces.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSWorldCollectable)

struct FInteractionQuery;

AHSWorldCollectable::AHSWorldCollectable()
{
}

void AHSWorldCollectable::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

FInventoryPickup AHSWorldCollectable::GetPickupInventory() const
{
	return StaticInventory;
}
