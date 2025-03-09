// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystem/Abilities/HSGameplayAbility.h"

#include "HSGameplayAbility_FromEquipment.generated.h"

class UHSEquipmentInstance;
class UHSInventoryItemInstance;

/**
 * UHSGameplayAbility_FromEquipment
 *
 * An ability granted by and associated with an equipment instance
 */
UCLASS()
class UHSGameplayAbility_FromEquipment : public UHSGameplayAbility
{
	GENERATED_BODY()

public:

	UHSGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="HS|Ability")
	UHSEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "HS|Ability")
	UHSInventoryItemInstance* GetAssociatedItem() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

};
