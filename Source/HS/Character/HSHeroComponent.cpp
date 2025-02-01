#include "HSHeroComponent.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Logging/MessageLog.h"
#include "HSLogChannels.h"
#include "EnhancedInputSubsystems.h"
#include "Player/HSPlayerController.h"
#include "Player/HSPlayerState.h"
//#include "Player/HSLocalPlayer.h"
#include "Character/HSPawnExtensionComponent.h"
#include "Character/HSPawnData.h"
#include "Character/HSCharacter.h"
#include "AbilitySystem/HSAbilitySystemComponent.h"
#include "Input/HSInputConfig.h"
#include "Input/HSInputComponent.h"
#include "Camera/HSCameraComponent.h"
#include "HSGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include "PlayerMappableInputConfig.h"
#include "Camera/HSCameraMode.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputMappingContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSHeroComponent)

#if WITH_EDITOR
#include "Misc/UObjectToken.h"
#endif	// WITH_EDITOR

namespace HSHero
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
};

const FName UHSHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UHSHeroComponent::NAME_ActorFeatureName("Hero");

UHSHeroComponent::UHSHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//AbilityCameraMode = nullptr;
	bReadyToBindInputs = false;
}

void UHSHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogHS, Error, TEXT("[UHSHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("HSHeroComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("HSHeroComponent");

			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));

			FMessageLog(HeroMessageLogName).Open();
		}
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

bool UHSHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == HSGameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == HSGameplayTags::InitState_Spawned && DesiredState == HSGameplayTags::InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<AHSPlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			AHSPlayerController* HSPC = GetController<AHSPlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !HSPC /*|| !HSPC->GetLocalPlayer()*/)
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == HSGameplayTags::InitState_DataAvailable && DesiredState == HSGameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		AHSPlayerState* HSPS = GetPlayerState<AHSPlayerState>();

		return HSPS && Manager->HasFeatureReachedInitState(Pawn, UHSPawnExtensionComponent::NAME_ActorFeatureName, HSGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == HSGameplayTags::InitState_DataInitialized && DesiredState == HSGameplayTags::InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void UHSHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == HSGameplayTags::InitState_DataAvailable && DesiredState == HSGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AHSPlayerState* HSPS = GetPlayerState<AHSPlayerState>();
		if (!ensure(Pawn && HSPS))
		{
			return;
		}

		const UHSPawnData* PawnData = nullptr;

		if (UHSPawnExtensionComponent* PawnExtComp = UHSPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UHSPawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			PawnExtComp->InitializeAbilitySystem(HSPS->GetHSAbilitySystemComponent(), HSPS);
		}

		if (AHSPlayerController* HSPC = GetController<AHSPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		// Hook up the delegate for all pawns, in case we spectate later
		if (PawnData)
		{
			if (UHSCameraComponent* CameraComponent = UHSCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void UHSHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UHSPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == HSGameplayTags::InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void UHSHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = { HSGameplayTags::InitState_Spawned, HSGameplayTags::InitState_DataAvailable, HSGameplayTags::InitState_DataInitialized, HSGameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UHSHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(UHSPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(HSGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UHSHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UHSHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	//const UHSLocalPlayer* LP = Cast<UHSLocalPlayer>(PC->GetLocalPlayer());
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const UHSPawnExtensionComponent* PawnExtComp = UHSPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UHSPawnData* PawnData = PawnExtComp->GetPawnData<UHSPawnData>())
		{
			if (const UHSInputConfig* InputConfig = PawnData->InputConfig)
			{
				for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
				{
					if (UInputMappingContext* IMC = Mapping.InputMapping.Get())
					{
						if (Mapping.bRegisterWithSettings)
						{
							if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
							{
								Settings->RegisterInputMappingContext(IMC);
							}

							FModifyContextOptions Options = {};
							Options.bIgnoreAllPressedKeysUntilRelease = false;
							// Actually add the config to the local player							
							Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
						}
					}
				}

				// The HS Input Component has some additional functions to map Gameplay Tags to an Input Action.
				// If you want this functionality but still want to change your input component class, make it a subclass
				// of the UHSInputComponent or modify this component accordingly.
				UHSInputComponent* HSIC = Cast<UHSInputComponent>(PlayerInputComponent);
				if (ensureMsgf(HSIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UHSInputComponent or a subclass of it.")))
				{
					// Add the key mappings that may have been set by the player
					HSIC->AddInputMappings(InputConfig, Subsystem);

					// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
					// be triggered directly by these input actions Triggered events. 
					TArray<uint32> BindHandles;
					HSIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

					HSIC->BindNativeAction(InputConfig, HSGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
					HSIC->BindNativeAction(InputConfig, HSGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
					HSIC->BindNativeAction(InputConfig, HSGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
					HSIC->BindNativeAction(InputConfig, HSGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
				}
			}
		}
	}

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UHSHeroComponent::AddAdditionalInputConfig(const UHSInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	//const ULocalPlayer* LP = PC->GetLocalPlayer();
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const UHSPawnExtensionComponent* PawnExtComp = UHSPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		UHSInputComponent* HSIC = Pawn->FindComponentByClass<UHSInputComponent>();
		if (ensureMsgf(HSIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UHSInputComponent or a subclass of it.")))
		{
			HSIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
		}
	}
}

void UHSHeroComponent::RemoveAdditionalInputConfig(const UHSInputConfig* InputConfig)
{
	//@TODO: Implement me!
}

bool UHSHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void UHSHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UHSPawnExtensionComponent* PawnExtComp = UHSPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UHSAbilitySystemComponent* HSASC = PawnExtComp->GetHSAbilitySystemComponent())
			{
				HSASC->AbilityInputTagPressed(InputTag);
			}
		}
	}
}

void UHSHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const UHSPawnExtensionComponent* PawnExtComp = UHSPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UHSAbilitySystemComponent* HSASC = PawnExtComp->GetHSAbilitySystemComponent())
		{
			HSASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UHSHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UHSHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UHSHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * HSHero::LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * HSHero::LookPitchRate * World->GetDeltaSeconds());
	}
}

void UHSHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (AHSCharacter* Character = GetPawn<AHSCharacter>())
	{
		Character->ToggleCrouch();
	}
}

TSubclassOf<UHSCameraMode> UHSHeroComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UHSPawnExtensionComponent* PawnExtComp = UHSPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UHSPawnData* PawnData = PawnExtComp->GetPawnData<UHSPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}

void UHSHeroComponent::SetAbilityCameraMode(TSubclassOf<UHSCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		AbilityCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void UHSHeroComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		AbilityCameraMode = nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}
