#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HSPawnData.generated.h"

class UHSAbilitySet;
class UHSAbilityTagRelationshipMapping;
class UHSInputConfig;

UCLASS(BlueprintType, Const, meta = (DisplayName = "HS Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class HS_API UHSPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UHSPawnData();

public:
	// Class to instantiate for this pawn (should usually derive from AHSPawn or AHSCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HS|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HS|Abilities")
	TArray<TObjectPtr<UHSAbilitySet>> AbilitySets;

	// What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HS|Abilities")
	TObjectPtr<UHSAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HS|Input")
	TObjectPtr<UHSInputConfig> InputConfig;

	//// Default camera mode used by player controlled pawns.
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HS|Camera")
	//TSubclassOf<UHSCameraMode> DefaultCameraMode;
};
