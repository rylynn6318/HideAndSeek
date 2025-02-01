#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "HSAttributeSet.generated.h"

class AActor;
class UHSAbilitySystemComponent;
struct FGameplayEffectSpec;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_MULTICAST_DELEGATE_SixParams(FHSAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/);

UCLASS()
class HS_API UHSAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UHSAttributeSet();

	UWorld* GetWorld() const override;

	UHSAbilitySystemComponent* GetHSAbilitySystemComponent() const;
};
