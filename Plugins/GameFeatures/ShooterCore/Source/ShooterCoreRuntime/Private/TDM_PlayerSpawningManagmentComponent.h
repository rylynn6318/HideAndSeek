// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Player/HSPlayerSpawningManagerComponent.h"

#include "TDM_PlayerSpawningManagmentComponent.generated.h"

class AActor;
class AController;
class AHSPlayerStart;
class UObject;

/**
 * 
 */
UCLASS()
class UTDM_PlayerSpawningManagmentComponent : public UHSPlayerSpawningManagerComponent
{
	GENERATED_BODY()

public:

	UTDM_PlayerSpawningManagmentComponent(const FObjectInitializer& ObjectInitializer);

	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<AHSPlayerStart*>& PlayerStarts) override;
	virtual void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation) override;

protected:

};
