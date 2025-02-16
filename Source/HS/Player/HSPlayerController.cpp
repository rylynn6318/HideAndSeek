#include "Player/HSPlayerController.h"
#include "Player/HSPlayerState.h"
#include "AbilitySystem/HSAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSPlayerController)

AHSPlayerController::AHSPlayerController(const FObjectInitializer& ObjectInitializer)
{
}

AHSPlayerState* AHSPlayerController::GetHSPlayerState() const
{
	return CastChecked<AHSPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UHSAbilitySystemComponent* AHSPlayerController::GetHSAbilitySystemComponent() const
{
	const AHSPlayerState* PS = GetHSPlayerState();
	return (PS ? PS->GetHSAbilitySystemComponent() : nullptr);
}

void AHSPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UHSAbilitySystemComponent* ASC = GetHSAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}
