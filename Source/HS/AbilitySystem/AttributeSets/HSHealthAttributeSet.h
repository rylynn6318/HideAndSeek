#pragma once

#include "CoreMinimal.h"
#include "HSAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "HSHealthAttributeSet.generated.h"

struct FGameplayEffectModCallbackData;

UCLASS()
class HS_API UHSHealthAttributeSet : public UHSAttributeSet
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(UHSHealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UHSHealthAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UHSHealthAttributeSet, Healing);
	ATTRIBUTE_ACCESSORS(UHSHealthAttributeSet, Damage);

protected:
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "HS|Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "HS|Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	// 체력 회복
	UPROPERTY(BlueprintReadOnly, Category = "HS|Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Healing;
	
	// 데미지
	UPROPERTY(BlueprintReadOnly, Category = "HS|Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	// 체력이 0이 되었을 때
	bool bOutOfHealth = false;
};
