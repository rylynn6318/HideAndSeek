// Copyright Epic Games, Inc. All Rights Reserved.

#include "HSEquipmentDefinition.h"
#include "HSEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSEquipmentDefinition)

UHSEquipmentDefinition::UHSEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UHSEquipmentInstance::StaticClass();
}

