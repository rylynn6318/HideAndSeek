#include "GameModes/HSGameState.h"
#include "HSGameState.h"
#include "AbilitySystem/HSAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSGameState)

AHSGameState::AHSGameState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UHSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

UAbilitySystemComponent* AHSGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UHSAbilitySystemComponent* AHSGameState::GetHSAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
