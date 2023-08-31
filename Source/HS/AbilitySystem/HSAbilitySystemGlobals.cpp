#include "AbilitySystem/HSAbilitySystemGlobals.h"
#include "AbilitySystem/HSAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSAbilitySystemGlobals)

UHSAbilitySystemComponent* UHSAbilitySystemGlobals::GetHSAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	if (Actor == nullptr)
	{
		return nullptr;
	}

	const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Actor);
	if (ASI)
	{
		return Cast<UHSAbilitySystemComponent>(ASI->GetAbilitySystemComponent());
	}

	if (LookForComponent)
	{
		// Fall back to a component search to better support BP-only actors
		return Actor->FindComponentByClass<UHSAbilitySystemComponent>();
	}

	return nullptr;
}
