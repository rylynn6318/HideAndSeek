#include "HSPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/HSAbilitySystemComponent.h"

AHSPlayerState::AHSPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UHSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

UAbilitySystemComponent* AHSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UHSAbilitySystemComponent* AHSPlayerState::GetHSAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
