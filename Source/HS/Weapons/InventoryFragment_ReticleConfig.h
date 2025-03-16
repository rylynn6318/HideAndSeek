// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/HSInventoryItemDefinition.h"

#include "InventoryFragment_ReticleConfig.generated.h"

class UHSReticleWidgetBase;
class UObject;

UCLASS()
class UInventoryFragment_ReticleConfig : public UHSInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Reticle)
	TArray<TSubclassOf<UHSReticleWidgetBase>> ReticleWidgets;
};
