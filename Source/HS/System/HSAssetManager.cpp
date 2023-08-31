#include "System/HSAssetManager.h"
#include "AbilitySystem/HSAbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSAssetManager)

void UHSAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UHSAbilitySystemGlobals::Get().InitGlobalData();
}
