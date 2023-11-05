#include "HSInputComponent.h"

#include "EnhancedInputSubsystems.h"
//#include "Player/HSLocalPlayer.h"
//#include "Settings/HSSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSInputComponent)

class UHSInputConfig;

UHSInputComponent::UHSInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UHSInputComponent::AddInputMappings(const UHSInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required
}

void UHSInputComponent::RemoveInputMappings(const UHSInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void UHSInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
