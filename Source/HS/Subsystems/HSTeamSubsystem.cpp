#include "Subsystems/HSTeamSubsystem.h"
#include "GenericTeamAgentInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSTeamSubsystem)

EHSTeamComparison UHSTeamSubsystem::CompareTeams(const UObject* A, const UObject* B, int32& TeamIdA, int32& TeamIdB) const
{
	TeamIdA = FindTeamFromObject(Cast<const AActor>(A));
	TeamIdB = FindTeamFromObject(Cast<const AActor>(B));

	if ((TeamIdA == INDEX_NONE) || (TeamIdB == INDEX_NONE))
	{
		return EHSTeamComparison::Invalid;
	}

	return (TeamIdA == TeamIdB) ? EHSTeamComparison::SameTeam : EHSTeamComparison::DifferentTeam;
}

EHSTeamComparison UHSTeamSubsystem::CompareTeams(const UObject* A, const UObject* B) const
{
	int32 TeamIdA;
	int32 TeamIdB;
	return CompareTeams(A, B, TeamIdA, TeamIdB);
}

int32 UHSTeamSubsystem::FindTeamFromObject(const UObject* Object) const
{
	if (const IGenericTeamAgentInterface* GenericTeamAgentInterface = Cast<IGenericTeamAgentInterface>(Object))
	{
		const FGenericTeamId ID = GenericTeamAgentInterface->GetGenericTeamId();
		return (ID == FGenericTeamId::NoTeam) ? INDEX_NONE : static_cast<int32>(ID);
	}

	return INDEX_NONE;
}
