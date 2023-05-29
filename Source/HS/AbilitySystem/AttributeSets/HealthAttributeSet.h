#pragma once

#include "CoreMinimal.h"
#include "HSAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "HealthAttributeSet.generated.h"

UCLASS()
class HS_API UHealthAttributeSet : public UHSAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, MaxHealth);

private:
	UPROPERTY(BlueprintReadOnly, Category = "HS|Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly, Category = "HS|Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
};
