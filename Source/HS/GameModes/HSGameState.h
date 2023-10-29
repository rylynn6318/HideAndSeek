#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "AbilitySystemInterface.h"
#include "HSGameState.generated.h"

class UAbilitySystemComponent;
class UHSAbilitySystemComponent;
class UHSExperienceManagerComponent;
class APlayerState;

struct FHSVerbMessage;

UCLASS(Config = Game)
class HS_API AHSGameState : public AModularGameStateBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AHSGameState();

	// AActor interface
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
	// ~AActor interface

	// AGameStateBase interface
	virtual void SeamlessTravelTransitionCheckpoint(bool bToTransitionMap) override;
	// ~AGameStateBase interface

	// IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~IAbilitySystemInterface interface

	UFUNCTION(BlueprintCallable, Category = "HS|GameState")
	virtual UHSAbilitySystemComponent* GetHSAbilitySystemComponent() const;

	// Send a message that all clients will (probably) get
	// (use only for client notifications like eliminations, server join messages, etc... that can handle being lost)
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "HS|GameState")
	void MulticastMessageToClients(const FHSVerbMessage Message);

	// Send a message that all clients will be guaranteed to get
	// (use only for client notifications that cannot handle being lost)
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "HS|GameState")
	void MulticastReliableMessageToClients(const FHSVerbMessage Message);

	// Gets the server's FPS, replicated to clients
	float GetServerFPS() const;

	// Indicate the local player state is recording a replay
	void SetRecorderPlayerState(APlayerState* NewPlayerState);

	// Gets the player state that recorded the replay, if valid
	APlayerState* GetRecorderPlayerState() const;

protected:
	UFUNCTION()
	void OnRep_RecorderPlayerState();

public:
	// Delegate called when the replay player state changes
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRecorderPlayerStateChanged, APlayerState*);
	FOnRecorderPlayerStateChanged OnRecorderPlayerStateChangedEvent;

protected:
	UPROPERTY(VisibleAnywhere, Category = "HS|Ability")
	UHSAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UHSExperienceManagerComponent> ExperienceManagerComponent;

	UPROPERTY(Replicated)
	float ServerFPS;

	// The player state that recorded a replay, it is used to select the right pawn to follow
	// This is only set in replay streams and is not replicated normally
	UPROPERTY(Transient, ReplicatedUsing = OnRep_RecorderPlayerState)
	TObjectPtr<APlayerState> RecorderPlayerState;
};
