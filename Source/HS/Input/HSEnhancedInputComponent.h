#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/HSInputConfig.h"
#include "HSEnhancedInputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;

UCLASS()
class HS_API UHSEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserClass, typename FuncType>
	void BindNativeAction(const UHSInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
	{
		check(InputConfig);
		if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
		{
			BindAction(IA, TriggerEvent, Object, Func);
		}
	}

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UHSInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
	{
		check(InputConfig);

		for (const FHSInputAction& Action : InputConfig->AbilityInputActions)
		{
			if (Action.InputAction && Action.InputTag.IsValid())
			{
				if (PressedFunc)
				{
					BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
				}

				if (ReleasedFunc)
				{
					BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
				}
			}
		}
	}
};
