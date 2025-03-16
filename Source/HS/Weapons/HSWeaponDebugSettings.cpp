// Copyright Epic Games, Inc. All Rights Reserved.

#include "HSWeaponDebugSettings.h"
#include "Misc/App.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSWeaponDebugSettings)

UHSWeaponDebugSettings::UHSWeaponDebugSettings()
{
}

FName UHSWeaponDebugSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}

