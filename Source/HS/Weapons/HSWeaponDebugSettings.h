// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettingsBackedByCVars.h"

#include "HSWeaponDebugSettings.generated.h"

class UObject;

/**
 * Developer debugging settings for weapons
 */
UCLASS(config=EditorPerProjectUserSettings)
class UHSWeaponDebugSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	UHSWeaponDebugSettings();

	//~UDeveloperSettings interface
	virtual FName GetCategoryName() const override;
	//~End of UDeveloperSettings interface

public:
	// Should we do debug drawing for bullet traces (if above zero, sets how long (in seconds)
	UPROPERTY(config, EditAnywhere, Category=General, meta=(ConsoleVariable="HS.Weapon.DrawBulletTraceDuration", ForceUnits=s))
	float DrawBulletTraceDuration;

	// Should we do debug drawing for bullet impacts (if above zero, sets how long (in seconds)
	UPROPERTY(config, EditAnywhere, Category = General, meta = (ConsoleVariable = "HS.Weapon.DrawBulletHitDuration", ForceUnits = s))
	float DrawBulletHitDuration;

	// When bullet hit debug drawing is enabled (see DrawBulletHitDuration), how big should the hit radius be? (in cm)
	UPROPERTY(config, EditAnywhere, Category = General, meta = (ConsoleVariable = "HS.Weapon.DrawBulletHitRadius", ForceUnits=cm))
	float DrawBulletHitRadius;
};
