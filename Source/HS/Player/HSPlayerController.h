#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HSPlayerController.generated.h"

class AHSPlayerState;
class UHSAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class HS_API AHSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHSPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "HS|PlayerController")
	AHSPlayerState* GetHSPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "HS|PlayerController")
	UHSAbilitySystemComponent* GetHSAbilitySystemComponent() const;

protected:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
