// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "HSTeamInfoBase.h"

#include "HSTeamPublicInfo.generated.h"

class UHSTeamCreationComponent;
class UHSTeamDisplayAsset;
class UObject;
struct FFrame;

UCLASS()
class AHSTeamPublicInfo : public AHSTeamInfoBase
{
	GENERATED_BODY()

	friend UHSTeamCreationComponent;

public:
	AHSTeamPublicInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UHSTeamDisplayAsset* GetTeamDisplayAsset() const { return TeamDisplayAsset; }

private:
	UFUNCTION()
	void OnRep_TeamDisplayAsset();

	void SetTeamDisplayAsset(TObjectPtr<UHSTeamDisplayAsset> NewDisplayAsset);

private:
	UPROPERTY(ReplicatedUsing=OnRep_TeamDisplayAsset)
	TObjectPtr<UHSTeamDisplayAsset> TeamDisplayAsset;
};
