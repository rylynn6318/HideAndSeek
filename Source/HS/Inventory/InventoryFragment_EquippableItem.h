// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/HSInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"

#include "InventoryFragment_EquippableItem.generated.h"

class UHSEquipmentDefinition;
class UObject;

UCLASS()
class UInventoryFragment_EquippableItem : public UHSInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=HS)
	TSubclassOf<UHSEquipmentDefinition> EquipmentDefinition;
};
