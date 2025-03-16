// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"

#include "HSWeaponUserInterface.generated.h"

class UHSWeaponInstance;
class UObject;
struct FGeometry;

UCLASS()
class UHSWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UHSWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(UHSWeaponInstance* OldWeapon, UHSWeaponInstance* NewWeapon);

private:
	void RebuildWidgetFromWeapon();

private:
	UPROPERTY(Transient)
	TObjectPtr<UHSWeaponInstance> CurrentInstance;
};
