#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "HSPlayerState.generated.h"

class UAbilitySystemComponent;
class UHSAbilitySystemComponent;
class UHSPawnData;
class UHSExperienceDefinition;

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

	template<typename T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	virtual void SetPawnData(const UHSPawnData* InPawnData);

protected:
	UFUNCTION()
	void OnRep_PawnData();

private:
	void OnExperienceLoaded(const UHSExperienceDefinition* CurrentExperience);

public:
	static const FName NAME_HSAbilityReady;

protected:
	UPROPERTY()
	TObjectPtr<UHSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const UHSPawnData> PawnData;
};
