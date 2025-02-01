#include "AbilitySystem/Attributes/HSAttributeSet.h"
#include "AbilitySystem/HSAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSAttributeSet)

UHSAttributeSet::UHSAttributeSet()
{
}

UWorld* UHSAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UHSAbilitySystemComponent* UHSAttributeSet::GetHSAbilitySystemComponent() const
{
	return Cast<UHSAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
