// Copyright Epic Games, Inc. All Rights Reserved.

#include "HSWeaponUserInterface.h"

#include "Equipment/HSEquipmentManagerComponent.h"
#include "GameFramework/Pawn.h"
#include "Weapons/HSWeaponInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSWeaponUserInterface)

struct FGeometry;

UHSWeaponUserInterface::UHSWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UHSWeaponUserInterface::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHSWeaponUserInterface::NativeDestruct()
{
	Super::NativeDestruct();
}

void UHSWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (UHSEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<UHSEquipmentManagerComponent>())
		{
			if (UHSWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<UHSWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					UHSWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					RebuildWidgetFromWeapon();
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}

void UHSWeaponUserInterface::RebuildWidgetFromWeapon()
{
	
}

