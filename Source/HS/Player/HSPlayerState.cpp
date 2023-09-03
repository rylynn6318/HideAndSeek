#include "HSPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/HSAbilitySystemComponent.h"
#include "AbilitySystem/HSAbilitySet.h"
#include "Character/HSPawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameModes/HSExperienceDefinition.h"
#include "GameModes/HSGameMode.h"
#include "Engine/World.h"
#include "HSLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSPlayerState)

const FName AHSPlayerState::NAME_HSAbilityReady("HSAbilitiesReady");

AHSPlayerState::AHSPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UHSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
}

void AHSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
}

UAbilitySystemComponent* AHSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UHSAbilitySystemComponent* AHSPlayerState::GetHSAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHSPlayerState::SetPawnData(const UHSPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogHS, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	for (const UHSAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_HSAbilityReady);

	ForceNetUpdate();
}

void AHSPlayerState::OnRep_PawnData()
{
}

void AHSPlayerState::OnExperienceLoaded(const UHSExperienceDefinition* CurrentExperience)
{
	if (AHSGameMode* HSGameMode = GetWorld()->GetAuthGameMode<AHSGameMode>())
	{
		if (const UHSPawnData* NewPawnData = HSGameMode->GetPawnDataForController(GetOwningController()))
		{
			SetPawnData(NewPawnData);
		}
		else
		{
			UE_LOG(LogHS, Error, TEXT("AHSPlayerState::OnExperienceLoaded(): Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
		}
	}
}
