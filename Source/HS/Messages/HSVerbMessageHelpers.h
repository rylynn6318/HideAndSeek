#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "HSVerbMessageHelpers.generated.h"

struct FGameplayCueParameters;
struct FHSVerbMessage;

class APlayerController;
class APlayerState;


UCLASS()
class HS_API UHSVerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HS")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "HS")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "HS")
	static FGameplayCueParameters VerbMessageToCueParameters(const FHSVerbMessage& Message);

	UFUNCTION(BlueprintCallable, Category = "HS")
	static FHSVerbMessage CueParametersToVerbMessage(const FGameplayCueParameters& Params);
};
