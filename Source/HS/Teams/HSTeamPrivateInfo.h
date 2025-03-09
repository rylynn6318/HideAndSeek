// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "HSTeamInfoBase.h"

#include "HSTeamPrivateInfo.generated.h"

class UObject;

UCLASS()
class AHSTeamPrivateInfo : public AHSTeamInfoBase
{
	GENERATED_BODY()

public:
	AHSTeamPrivateInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
