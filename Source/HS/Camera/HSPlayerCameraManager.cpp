// Copyright Epic Games, Inc. All Rights Reserved.

#include "HSPlayerCameraManager.h"

#include "Async/TaskGraphInterfaces.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "HSCameraComponent.h"
#include "HSUICameraManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSPlayerCameraManager)

class FDebugDisplayInfo;

static FName UICameraComponentName(TEXT("UICamera"));

AHSPlayerCameraManager::AHSPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = HS_CAMERA_DEFAULT_FOV;
	ViewPitchMin = HS_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = HS_CAMERA_DEFAULT_PITCH_MAX;

	UICamera = CreateDefaultSubobject<UHSUICameraManagerComponent>(UICameraComponentName);
}

UHSUICameraManagerComponent* AHSPlayerCameraManager::GetUICameraComponent() const
{
	return UICamera;
}

void AHSPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	// If the UI Camera is looking at something, let it have priority.
	if (UICamera->NeedsToUpdateViewTarget())
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
		UICamera->UpdateViewTarget(OutVT, DeltaTime);
		return;
	}

	Super::UpdateViewTarget(OutVT, DeltaTime);
}

void AHSPlayerCameraManager::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("HSPlayerCameraManager: %s"), *GetNameSafe(this)));

	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);

	const APawn* Pawn = (PCOwner ? PCOwner->GetPawn() : nullptr);

	if (const UHSCameraComponent* CameraComponent = UHSCameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->DrawDebug(Canvas);
	}
}

