// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "HSAimSensitivityData.generated.h"

enum class EHSGamepadSensitivity : uint8;

class UObject;

/** Defines a set of gamepad sensitivity to a float value. */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "HS Aim Sensitivity Data", ShortTooltip = "Data asset used to define a map of Gamepad Sensitivty to a float value."))
class HS_API UHSAimSensitivityData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UHSAimSensitivityData(const FObjectInitializer& ObjectInitializer);
	
	const float SensitivtyEnumToFloat(const EHSGamepadSensitivity InSensitivity) const;
	
protected:
	/** Map of SensitivityMap settings to their corresponding float */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EHSGamepadSensitivity, float> SensitivityMap;
};
