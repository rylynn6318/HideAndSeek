#include "System/HSAssetManager.h"
#include "AbilitySystem/HSAbilitySystemGlobals.h"

void UHSAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UHSAbilitySystemGlobals::Get().InitGlobalData();
}
