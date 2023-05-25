#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "HSPlayerState.generated.h"

class UAbilitySystemComponent;
class UHSAbilitySystemComponent;

UCLASS()
class HS_API AHSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AHSPlayerState();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~IAbilitySystemInterface
	
	virtual UHSAbilitySystemComponent* GetHSAbilitySystemComponent() const;

protected:
	UPROPERTY()
	TObjectPtr<UHSAbilitySystemComponent> AbilitySystemComponent;
};
