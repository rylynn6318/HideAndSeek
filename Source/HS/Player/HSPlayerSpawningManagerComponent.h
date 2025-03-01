// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "HSPlayerSpawningManagerComponent.generated.h"

class AController;
class APlayerController;
class APlayerState;
class APlayerStart;
class AHSPlayerStart;
class AActor;

UCLASS()
class HS_API UHSPlayerSpawningManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UHSPlayerSpawningManagerComponent(const FObjectInitializer& ObjectInitializer);

	/** UActorComponent */
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	/** ~UActorComponent */

protected:
	// Utility
	APlayerStart* GetFirstRandomUnoccupiedPlayerStart(AController* Controller, const TArray<AHSPlayerStart*>& FoundStartPoints) const;

	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<AHSPlayerStart*>& PlayerStarts) { return nullptr; }
	virtual void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation) { }

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = OnFinishRestartPlayer))
	void BP_OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation);

private:
	/** We proxy these calls from AHSGameMode, to this component so that each experience can more easily customize the respawn system they want. */
	AActor* ChoosePlayerStart(AController* Player);
	bool ControllerCanRestart(AController* Player);
	void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation);
	friend class AHSGameMode;
	/** ~AHSGameMode */

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AHSPlayerStart>> CachedPlayerStarts;

private:
	void OnLevelAdded(ULevel* InLevel, UWorld* InWorld);
	void HandleOnActorSpawned(AActor* SpawnedActor);

#if WITH_EDITOR
	APlayerStart* FindPlayFromHereStart(AController* Player);
#endif
};
