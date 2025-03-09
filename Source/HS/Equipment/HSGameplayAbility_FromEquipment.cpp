// Copyright Epic Games, Inc. All Rights Reserved.

#include "HSGameplayAbility_FromEquipment.h"
#include "HSEquipmentInstance.h"
#include "Inventory/HSInventoryItemInstance.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSGameplayAbility_FromEquipment)

UHSGameplayAbility_FromEquipment::UHSGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UHSEquipmentInstance* UHSGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UHSEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}

UHSInventoryItemInstance* UHSGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UHSEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UHSInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}


#if WITH_EDITOR
EDataValidationResult UHSGameplayAbility_FromEquipment::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

PRAGMA_DISABLE_DEPRECATION_WARNINGS
	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
PRAGMA_ENABLE_DEPRECATION_WARNINGS
	{
		Context.AddError(NSLOCTEXT("HS", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}

#endif
