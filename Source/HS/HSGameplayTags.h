#pragma once

#include "NativeGameplayTags.h"

namespace HSGameplayTags
{
	HS_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// Declare all of the custom native tags that Lyra will use
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ABILITY_SIMPLE_FAILURE_MESSAGE);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_ABILITY_PLAY_MONTAGE_FAILURE_MESSAGE);

	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);

	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Reset);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_RequestReset);

	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Heal);

	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_GodMode);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_UnlimitedHealth);

	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_AutoRunning);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);

	HS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Damage);
	HS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity);
	HS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageSelfDestruct);
	HS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_FellOutOfWorld);
	HS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_MovementStopped);
	HS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityInputBlocked);
	HS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_HS_Damage_Message);

	// These are mappings from MovementMode enums to GameplayTags associated with those enums (below)
	HS_API	extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	HS_API	extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_NavWalking);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Swimming);
	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);

	HS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Custom);
};
