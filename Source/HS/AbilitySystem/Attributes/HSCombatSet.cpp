#include "HSCombatSet.h"

#include "AbilitySystem/Attributes/HSAttributeSet.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSCombatSet)

class FLifetimeProperty;


UHSCombatSet::UHSCombatSet()
	: BaseDamage(0.0f)
	, BaseHeal(0.0f)
{
}

void UHSCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHSCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHSCombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}

void UHSCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSCombatSet, BaseDamage, OldValue);
}

void UHSCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSCombatSet, BaseHeal, OldValue);
}

