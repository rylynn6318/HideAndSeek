//// Copyright Epic Games, Inc. All Rights Reserved.
//
//#include "Player/HSLocalPlayer.h"
//
//#include "AudioMixerBlueprintLibrary.h"
//#include "Engine/World.h"
//#include "GameFramework/PlayerController.h"
//#include "Settings/HSSettingsLocal.h"
//#include "Settings/HSSettingsShared.h"
//#include "CommonUserSubsystem.h"
//
//#include UE_INLINE_GENERATED_CPP_BY_NAME(HSLocalPlayer)
//
//class UObject;
//
//UHSLocalPlayer::UHSLocalPlayer()
//{
//}
//
//void UHSLocalPlayer::PostInitProperties()
//{
//	Super::PostInitProperties();
//
//	if (UHSSettingsLocal* LocalSettings = GetLocalSettings())
//	{
//		LocalSettings->OnAudioOutputDeviceChanged.AddUObject(this, &UHSLocalPlayer::OnAudioOutputDeviceChanged);
//	}
//}
//
//void UHSLocalPlayer::SwitchController(class APlayerController* PC)
//{
//	Super::SwitchController(PC);
//
//	OnPlayerControllerChanged(PlayerController);
//}
//
//bool UHSLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
//{
//	const bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);
//
//	OnPlayerControllerChanged(PlayerController);
//
//	return bResult;
//}
//
//void UHSLocalPlayer::InitOnlineSession()
//{
//	OnPlayerControllerChanged(PlayerController);
//
//	Super::InitOnlineSession();
//}
//
//void UHSLocalPlayer::OnPlayerControllerChanged(APlayerController* NewController)
//{
//	// Stop listening for changes from the old controller
//	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
//	if (IHSTeamAgentInterface* ControllerAsTeamProvider = Cast<IHSTeamAgentInterface>(LastBoundPC.Get()))
//	{
//		OldTeamID = ControllerAsTeamProvider->GetGenericTeamId();
//		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().RemoveAll(this);
//	}
//
//	// Grab the current team ID and listen for future changes
//	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
//	if (IHSTeamAgentInterface* ControllerAsTeamProvider = Cast<IHSTeamAgentInterface>(NewController))
//	{
//		NewTeamID = ControllerAsTeamProvider->GetGenericTeamId();
//		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnControllerChangedTeam);
//		LastBoundPC = NewController;
//	}
//
//	ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
//}
//
//void UHSLocalPlayer::SetGenericTeamId(const FGenericTeamId& NewTeamID)
//{
//	// Do nothing, we merely observe the team of our associated player controller
//}
//
//FGenericTeamId UHSLocalPlayer::GetGenericTeamId() const
//{
//	if (IHSTeamAgentInterface* ControllerAsTeamProvider = Cast<IHSTeamAgentInterface>(PlayerController))
//	{
//		return ControllerAsTeamProvider->GetGenericTeamId();
//	}
//	else
//	{
//		return FGenericTeamId::NoTeam;
//	}
//}
//
//FOnHSTeamIndexChangedDelegate* UHSLocalPlayer::GetOnTeamIndexChangedDelegate()
//{
//	return &OnTeamChangedDelegate;
//}
//
//UHSSettingsLocal* UHSLocalPlayer::GetLocalSettings() const
//{
//	return UHSSettingsLocal::Get();
//}
//
//UHSSettingsShared* UHSLocalPlayer::GetSharedSettings() const
//{
//	if (!SharedSettings)
//	{
//		// On PC it's okay to use the sync load because it only checks the disk
//		// This could use a platform tag to check for proper save support instead
//		bool bCanLoadBeforeLogin = PLATFORM_DESKTOP;
//		
//		if (bCanLoadBeforeLogin)
//		{
//			SharedSettings = UHSSettingsShared::LoadOrCreateSettings(this);
//		}
//		else
//		{
//			// We need to wait for user login to get the real settings so return temp ones
//			SharedSettings = UHSSettingsShared::CreateTemporarySettings(this);
//		}
//	}
//
//	return SharedSettings;
//}
//
//void UHSLocalPlayer::LoadSharedSettingsFromDisk(bool bForceLoad)
//{
//	FUniqueNetIdRepl CurrentNetId = GetCachedUniqueNetId();
//	if (!bForceLoad && SharedSettings && CurrentNetId == NetIdForSharedSettings)
//	{
//		// Already loaded once, don't reload
//		return;
//	}
//
//	ensure(UHSSettingsShared::AsyncLoadOrCreateSettings(this, UHSSettingsShared::FOnSettingsLoadedEvent::CreateUObject(this, &UHSLocalPlayer::OnSharedSettingsLoaded)));
//}
//
//void UHSLocalPlayer::OnSharedSettingsLoaded(UHSSettingsShared* LoadedOrCreatedSettings)
//{
//	// The settings are applied before it gets here
//	if (ensure(LoadedOrCreatedSettings))
//	{
//		// This will replace the temporary or previously loaded object which will GC out normally
//		SharedSettings = LoadedOrCreatedSettings;
//
//		NetIdForSharedSettings = GetCachedUniqueNetId();
//	}
//}
//
//void UHSLocalPlayer::OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId)
//{
//	FOnCompletedDeviceSwap DevicesSwappedCallback;
//	DevicesSwappedCallback.BindUFunction(this, FName("OnCompletedAudioDeviceSwap"));
//	UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(GetWorld(), InAudioOutputDeviceId, DevicesSwappedCallback);
//}
//
//void UHSLocalPlayer::OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult)
//{
//	if (SwapResult.Result == ESwapAudioOutputDeviceResultState::Failure)
//	{
//	}
//}
//
//void UHSLocalPlayer::OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
//{
//	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
//}
//
