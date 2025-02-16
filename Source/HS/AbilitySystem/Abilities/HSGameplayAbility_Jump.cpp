#include "HSGameplayAbility_Jump.h"

#include "AbilitySystem/Abilities/HSGameplayAbility.h"
#include "Character/HSCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSGameplayAbility_Jump)

struct FGameplayTagContainer;

UHSGameplayAbility_Jump::UHSGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UHSGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const AHSCharacter* HSCharacter = Cast<AHSCharacter>(ActorInfo->AvatarActor.Get());
	if (!HSCharacter || !HSCharacter->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UHSGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CharacterJumpStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHSGameplayAbility_Jump::CharacterJumpStart()
{
	if (AHSCharacter* HSCharacter = GetHSCharacterFromActorInfo())
	{
		if (HSCharacter->IsLocallyControlled() && !HSCharacter->bPressedJump)
		{
			HSCharacter->UnCrouch();
			HSCharacter->Jump();
		}
	}
}

void UHSGameplayAbility_Jump::CharacterJumpStop()
{
	if (AHSCharacter* HSCharacter = GetHSCharacterFromActorInfo())
	{
		if (HSCharacter->IsLocallyControlled() && HSCharacter->bPressedJump)
		{
			HSCharacter->StopJumping();
		}
	}
}

