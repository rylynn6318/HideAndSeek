#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "HSTeamSubsystem.generated.h"

UENUM(BlueprintType)
enum class EHSTeamComparison : uint8
{
	SameTeam,
	DifferentTeam,
	Invalid
};

UCLASS()
class HS_API UHSTeamSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	int32 FindTeamFromObject(const UObject* Object) const;

	UFUNCTION(BlueprintCallable, Category="Teams", meta=(ExpandEnumAsExecs=ReturnValue))
	EHSTeamComparison CompareTeams(const UObject* A, const UObject* B, int32& TeamIdA, int32& TeamIdB) const;
	EHSTeamComparison CompareTeams(const UObject* A, const UObject* B) const;
};
