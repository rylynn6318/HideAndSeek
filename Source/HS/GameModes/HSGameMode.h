#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "HSGameMode.generated.h"

class UHSPawnData;
class UHSExperienceDefinition;

UCLASS()
class AHSGameMode : public AModularGameModeBase
{
	GENERATED_BODY()

public:
	AHSGameMode();

	const UHSPawnData* GetPawnDataForController(const AController* InController) const;

	// AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	//virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	//virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	//virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	//virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	//virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	//virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
	//virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	//virtual void InitGameState() override;
	//virtual bool UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage) override;
	//virtual void GenericPlayerInitialization(AController* NewPlayer) override;
	//virtual void FailedToRestartPlayer(AController* NewPlayer) override;
	// ~AGameModeBase interface

protected:
	void OnExperienceLoaded(const UHSExperienceDefinition* CurrentExperience);
	bool IsExperienceLoaded() const;

	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource);

	void HandleMatchAssignmentIfNotExpectingOne();
};



