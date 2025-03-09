// Copyright Epic Games, Inc. All Rights Reserved.

#include "HSAimSensitivityData.h"

#include "Settings/HSSettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSAimSensitivityData)

UHSAimSensitivityData::UHSAimSensitivityData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SensitivityMap =
	{
		{ EHSGamepadSensitivity::Slow,			0.5f },
		{ EHSGamepadSensitivity::SlowPlus,		0.75f },
		{ EHSGamepadSensitivity::SlowPlusPlus,	0.9f },
		{ EHSGamepadSensitivity::Normal,		1.0f },
		{ EHSGamepadSensitivity::NormalPlus,	1.1f },
		{ EHSGamepadSensitivity::NormalPlusPlus,1.25f },
		{ EHSGamepadSensitivity::Fast,			1.5f },
		{ EHSGamepadSensitivity::FastPlus,		1.75f },
		{ EHSGamepadSensitivity::FastPlusPlus,	2.0f },
		{ EHSGamepadSensitivity::Insane,		2.5f },
	};
}

const float UHSAimSensitivityData::SensitivtyEnumToFloat(const EHSGamepadSensitivity InSensitivity) const
{
	if (const float* Sens = SensitivityMap.Find(InSensitivity))
	{
		return *Sens;
	}

	return 1.0f;
}

