#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "Teams/HSTeamAgentInterface.h"

#include "HSLocalPlayer.generated.h"

struct FGenericTeamId;

class APlayerController;
class UInputMappingContext;
class UHSSettingsLocal;
class UHSSettingsShared;
class UObject;
class UWorld;
struct FFrame;
struct FSwapAudioOutputResult;

/**
 * UHSLocalPlayer
 */
UCLASS()
class HS_API UHSLocalPlayer : public UCommonLocalPlayer, public IHSTeamAgentInterface
{
	GENERATED_BODY()

public:
	UHSLocalPlayer();

	//~UObject interface
	virtual void PostInitProperties() override;
	//~End of UObject interface

	//~UPlayer interface
	virtual void SwitchController(class APlayerController* PC) override;
	//~End of UPlayer interface

	//~ULocalPlayer interface
	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
	virtual void InitOnlineSession() override;
	//~End of ULocalPlayer interface

	//~IHSTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnHSTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of IHSTeamAgentInterface interface

	/** Gets the local settings for this player, this is read from config files at process startup and is always valid */
	UFUNCTION()
	UHSSettingsLocal* GetLocalSettings() const;

	/** Gets the shared setting for this player, this is read using the save game system so may not be correct until after user login */
	UFUNCTION()
	UHSSettingsShared* GetSharedSettings() const;

	/** Starts an async request to load the shared settings, this will call OnSharedSettingsLoaded after loading or creating new ones */
	void LoadSharedSettingsFromDisk(bool bForceLoad = false);

protected:
	void OnSharedSettingsLoaded(UHSSettingsShared* LoadedOrCreatedSettings);

	void OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId);
	
	UFUNCTION()
	void OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult);

	void OnPlayerControllerChanged(APlayerController* NewController);

	UFUNCTION()
	void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

private:
	UPROPERTY(Transient)
	mutable TObjectPtr<UHSSettingsShared> SharedSettings;

	FUniqueNetIdRepl NetIdForSharedSettings;

	UPROPERTY(Transient)
	mutable TObjectPtr<const UInputMappingContext> InputMappingContext;

	UPROPERTY()
	FOnHSTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> LastBoundPC;
};
