#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "AbilitySystemInterface.h"
#include "HSGameState.generated.h"

class UAbilitySystemComponent;
class UHSAbilitySystemComponent;

UCLASS()
class HS_API AHSGameState : public AModularGameStateBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AHSGameState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UHSAbilitySystemComponent* GetHSAbilitySystemComponent() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "HS|Ability")
	UHSAbilitySystemComponent* AbilitySystemComponent;
};
