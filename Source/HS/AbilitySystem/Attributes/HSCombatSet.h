#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "HSAttributeSet.h"

#include "HSCombatSet.generated.h"

/**
 * UHSCombatSet
 *
 *  Class that defines attributes that are necessary for applying damage or healing.
 *	Attribute examples include: damage, healing, attack power, and shield penetrations.
 */
UCLASS(BlueprintType)
class UHSCombatSet : public UHSAttributeSet
{
	GENERATED_BODY()

public:
	UHSCombatSet();

	ATTRIBUTE_ACCESSORS(UHSCombatSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UHSCombatSet, BaseHeal);

protected:
	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);

private:
	// The base amount of damage to apply in the damage execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "HS|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	// The base amount of healing to apply in the heal execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseHeal, Category = "HS|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;
};
