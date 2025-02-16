#pragma once

#include "CoreMinimal.h"
#include "HSGameplayAbility.h"
#include "HSGameplayAbility_Jump.generated.h"

struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;

UCLASS(Abstract)
class UHSGameplayAbility_Jump : public UHSGameplayAbility
{
	GENERATED_BODY()

public:
	UHSGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "HS|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "HS|Ability")
	void CharacterJumpStop();
};
