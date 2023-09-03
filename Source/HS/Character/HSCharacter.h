// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystemInterface.h"
#include "HSCharacter.generated.h"

class UHSInputConfig;
class UHSAbilitySystemComponent;

UCLASS(config=Game)
class AHSCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	AHSCharacter();

	// IGenericTeamAgentInterface Interface
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	// ~IGenericTeamAgentInterface Interface

	// APawn Interface
	virtual void OnRep_PlayerState() override;
	virtual void PossessedBy(AController* NewController) override;
	// ~APawn Interface

	// IAbilitySystemInterface Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~IAbilitySystemInterface Interface
	UHSAbilitySystemComponent* GetHSAbilitySystemComponent() const;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Teams")
	FGenericTeamId TeamId = FGenericTeamId::NoTeam;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UHSInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "HS|Ability")
	TObjectPtr<UHSAbilitySystemComponent> AbilitySystemComponent;
};

