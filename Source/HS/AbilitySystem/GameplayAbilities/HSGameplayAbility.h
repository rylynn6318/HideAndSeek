#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "HSGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EHSAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

UCLASS()
class HS_API UHSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	EHSAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EHSAbilityActivationPolicy ActivationPolicy;
};
