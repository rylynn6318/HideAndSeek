#include "HSGameMode.h"
#include "GameModes/HSGameState.h"
#include "Player/HSPlayerController.h"
#include "Player/HSPlayerState.h"
#include "Character/HSCharacter.h"
#include "Character/HSPawnData.h"
#include "GameModes/HSExperienceManagerComponent.h"
#include "GameModes/HSExperienceDefinition.h"
#include "System/HSAssetManager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HSLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSGameMode)

AHSGameMode::AHSGameMode()
{
	GameStateClass = AHSGameState::StaticClass();
	PlayerControllerClass = AHSPlayerController::StaticClass();
	PlayerStateClass = AHSPlayerState::StaticClass();
	DefaultPawnClass = AHSCharacter::StaticClass();
}

const UHSPawnData* AHSGameMode::GetPawnDataForController(const AController* InController) const
{
	// See if pawn data is already set on the player state
	if (InController != nullptr)
	{
		if (const AHSPlayerState* HSPS = InController->GetPlayerState<AHSPlayerState>())
		{
			if (const UHSPawnData* PawnData = HSPS->GetPawnData<UHSPawnData>())
			{
				return PawnData;
			}
		}
	}

	// If not, fall back to the the default for the current experience
	check(GameState);
	UHSExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UHSExperienceManagerComponent>();
	check(ExperienceComponent);

	if (ExperienceComponent->IsExperienceLoaded())
	{
		const UHSExperienceDefinition* Experience = ExperienceComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData != nullptr)
		{
			return Experience->DefaultPawnData;
		}

		// Experience is loaded and there's still no pawn data, fall back to the default for now
		return UHSAssetManager::Get().GetDefaultPawnData();
	}

	// Experience not loaded yet, so there is no pawn data to be had
	return nullptr;
}

void AHSGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Wait for the next frame to give time to initialize startup settings
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void AHSGameMode::OnExperienceLoaded(const UHSExperienceDefinition* CurrentExperience)
{	
	// Spawn any players that are already attached
	//@TODO: Here we're handling only *player* controllers, but in GetDefaultPawnClassForController_Implementation we skipped all controllers
	// GetDefaultPawnClassForController_Implementation might only be getting called for players anyways
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if ((PC != nullptr) && (PC->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

bool AHSGameMode::IsExperienceLoaded() const
{
	return false;
}

void AHSGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource)
{
	if (ExperienceId.IsValid())
	{
		UE_LOG(LogHSExperience, Log, TEXT("Identified experience %s (Source: %s)"), *ExperienceId.ToString(), *ExperienceIdSource);

		UHSExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UHSExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->SetCurrentExperience(ExperienceId);
	}
	else
	{
		UE_LOG(LogHSExperience, Error, TEXT("Failed to identify experience, loading screen will stay up forever"));
	}
}

void AHSGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	FString ExperienceIdSource;

	// Precedence order (highest wins)
	//  - Matchmaking assignment (if present)
	//  - URL Options override
	//  - Developer Settings (PIE only)
	//  - Command Line override
	//  - World Settings
	//  - Dedicated server
	//  - Default experience

	// 일단은 디폴트로 항상 실행
	ExperienceId = FPrimaryAssetId(FPrimaryAssetType("HSExperienceDefinition"), FName("BP_HSDefaultExperience"));
	ExperienceIdSource = TEXT("Default");

	OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
}
