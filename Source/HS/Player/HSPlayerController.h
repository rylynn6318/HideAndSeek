#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Teams/HSTeamAgentInterface.h"
#include "HSPlayerController.generated.h"

class AHSPlayerState;
class UHSAbilitySystemComponent;
class AHSHUD;

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

	UFUNCTION(BlueprintCallable, Category = "HS|PlayerController")
	AHSHUD* GetHSHUD() const;

	// AActor interface
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~AActor interface

	// AController interface
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	// ~AController interface

	// APlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetPlayer(UPlayer* InPlayer) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~APlayerController interface

protected:
	virtual void OnPlayerStateChanged();

private:
	void BroadcastOnPlayerStateChanged();

	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

private:
	UPROPERTY()
	FOnHSTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;
};
