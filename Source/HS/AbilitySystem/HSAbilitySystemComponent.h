#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "HSAbilitySystemComponent.generated.h"

class UHSAbilityTagRelationshipMapping;

UCLASS()
class HS_API UHSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	/** Sets the current tag relationship mapping, if null it will clear it out */
	void SetTagRelationshipMapping(UHSAbilityTagRelationshipMapping* NewMapping);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

protected:
	// If set, this table is used to look up tag relationships for activate and cancel
	UPROPERTY()
	TObjectPtr<UHSAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
