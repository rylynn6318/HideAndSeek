#include "Player/HSPlayerController.h"
#include "Player/HSPlayerState.h"
#include "AbilitySystem/HSAbilitySystemComponent.h"
#include "UI/HSHUD.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemGlobals.h"
#include "Teams/HSTeamAgentInterface.h"

#if WITH_RPC_REGISTRY
#include "Tests/HSGameplayRpcRegistrationComponent.h"
#include "HttpServerModule.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSPlayerController)

namespace HS
{
	namespace Input
	{
		static int32 ShouldAlwaysPlayForceFeedback = 0;
		static FAutoConsoleVariableRef CVarShouldAlwaysPlayForceFeedback(TEXT("HSPC.ShouldAlwaysPlayForceFeedback"),
			ShouldAlwaysPlayForceFeedback,
			TEXT("Should force feedback effects be played, even if the last input device was not a gamepad?"));
	}
}

AHSPlayerController::AHSPlayerController(const FObjectInitializer& ObjectInitializer)
{
}

AHSPlayerState* AHSPlayerController::GetHSPlayerState() const
{
	return CastChecked<AHSPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UHSAbilitySystemComponent* AHSPlayerController::GetHSAbilitySystemComponent() const
{
	const AHSPlayerState* PS = GetHSPlayerState();
	return (PS ? PS->GetHSAbilitySystemComponent() : nullptr);
}

AHSHUD* AHSPlayerController::GetHSHUD() const
{
	return CastChecked<AHSHUD>(GetHUD(), ECastCheckedType::NullAllowed);
}

void AHSPlayerController::BeginPlay()
{
	Super::BeginPlay();
#if WITH_RPC_REGISTRY
	FHttpServerModule::Get().StartAllListeners();
	int32 RpcPort = 0;
	if (FParse::Value(FCommandLine::Get(), TEXT("rpcport="), RpcPort))
	{
		UHSGameplayRpcRegistrationComponent* ObjectInstance = UHSGameplayRpcRegistrationComponent::GetInstance();
		if (ObjectInstance && ObjectInstance->IsValidLowLevel())
		{
			ObjectInstance->RegisterAlwaysOnHttpCallbacks();
			ObjectInstance->RegisterInMatchHttpCallbacks();
		}
	}
#endif
	SetActorHiddenInGame(false);
}

void AHSPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Disable replicating the PC target view as it doesn't work well for replays or client-side spectating.
	// The engine TargetViewRotation is only set in APlayerController::TickActor if the server knows ahead of time that 
	// a specific pawn is being spectated and it only replicates down for COND_OwnerOnly.
	// In client-saved replays, COND_OwnerOnly is never true and the target pawn is not always known at the time of recording.
	// To support client-saved replays, the replication of this was moved to ReplicatedViewRotation and updated in PlayerTick.
	DISABLE_REPLICATED_PROPERTY(APlayerController, TargetViewRotation);
}

void AHSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

#if WITH_SERVER_CODE && WITH_EDITOR
	//if (GIsEditor && (InPawn != nullptr) && (GetPawn() == InPawn))
	//{
	//	for (const FHSCheatToRun& CheatRow : GetDefault<UHSDeveloperSettings>()->CheatsToRun)
	//	{
	//		if (CheatRow.Phase == ECheatExecutionTime::OnPlayerPawnPossession)
	//		{
	//			ConsoleCommand(CheatRow.Cheat, /*bWriteToLog=*/ true);
	//		}
	//	}
	//}
#endif
}

void AHSPlayerController::OnUnPossess()
{
	// Make sure the pawn that is being unpossessed doesn't remain our ASC's avatar actor
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}

	Super::OnUnPossess();
}

void AHSPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AHSPlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AHSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	BroadcastOnPlayerStateChanged();
}

void AHSPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//if (GetIsAutoRunning())
	//{
	//	if (APawn* CurrentPawn = GetPawn())
	//	{
	//		const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	//		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
	//		CurrentPawn->AddMovementInput(MovementDirection, 1.0f);
	//	}
	//}

	AHSPlayerState* HSPlayerState = GetHSPlayerState();

	if (PlayerCameraManager && HSPlayerState)
	{
		APawn* TargetPawn = PlayerCameraManager->GetViewTargetPawn();

		if (TargetPawn)
		{
			// Update view rotation on the server so it replicates
			if (HasAuthority() || TargetPawn->IsLocallyControlled())
			{
				HSPlayerState->SetReplicatedViewRotation(TargetPawn->GetViewRotation());
			}

			// Update the target view rotation if the pawn isn't locally controlled
			if (!TargetPawn->IsLocallyControlled())
			{
				HSPlayerState = TargetPawn->GetPlayerState<AHSPlayerState>();
				if (HSPlayerState)
				{
					// Get it from the spectated pawn's player state, which may not be the same as the PC's playerstate
					TargetViewRotation = HSPlayerState->GetReplicatedViewRotation();
				}
			}
		}
	}
}

void AHSPlayerController::SetPlayer(UPlayer* InPlayer)
{
	Super::SetPlayer(InPlayer);

	//if (const UHSLocalPlayer* HSLocalPlayer = Cast<UHSLocalPlayer>(InPlayer))
	//{
	//	UHSSettingsShared* UserSettings = HSLocalPlayer->GetSharedSettings();
	//	UserSettings->OnSettingChanged.AddUObject(this, &ThisClass::OnSettingsChanged);

	//	OnSettingsChanged(UserSettings);
	//}
}

void AHSPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UHSAbilitySystemComponent* ASC = GetHSAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void AHSPlayerController::OnPlayerStateChanged()
{
}

void AHSPlayerController::BroadcastOnPlayerStateChanged()
{
	OnPlayerStateChanged();

	// Unbind from the old player state, if any
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	if (LastSeenPlayerState != nullptr)
	{
		if (IHSTeamAgentInterface* PlayerStateTeamInterface = Cast<IHSTeamAgentInterface>(LastSeenPlayerState))
		{
			OldTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().RemoveAll(this);
		}
	}

	// Bind to the new player state, if any
	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (PlayerState != nullptr)
	{
		if (IHSTeamAgentInterface* PlayerStateTeamInterface = Cast<IHSTeamAgentInterface>(PlayerState))
		{
			NewTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnPlayerStateChangedTeam);
		}
	}

	// Broadcast the team change (if it really has)
	IHSTeamAgentInterface::ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);

	LastSeenPlayerState = PlayerState;
}

void AHSPlayerController::OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	IHSTeamAgentInterface::ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
}
