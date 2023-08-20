

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "HSAbilitySystemGlobals.generated.h"

class UHSAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class HS_API UHSAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:
	static UHSAbilitySystemComponent* GetHSAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = true);
};
