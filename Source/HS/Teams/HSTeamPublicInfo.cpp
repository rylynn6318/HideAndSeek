// Copyright Epic Games, Inc. All Rights Reserved.

#include "HSTeamPublicInfo.h"

#include "Net/UnrealNetwork.h"
#include "Teams/HSTeamInfoBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSTeamPublicInfo)

class FLifetimeProperty;

AHSTeamPublicInfo::AHSTeamPublicInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AHSTeamPublicInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, TeamDisplayAsset, COND_InitialOnly);
}

void AHSTeamPublicInfo::SetTeamDisplayAsset(TObjectPtr<UHSTeamDisplayAsset> NewDisplayAsset)
{
	check(HasAuthority());
	check(TeamDisplayAsset == nullptr);

	TeamDisplayAsset = NewDisplayAsset;

	TryRegisterWithTeamSubsystem();
}

void AHSTeamPublicInfo::OnRep_TeamDisplayAsset()
{
	TryRegisterWithTeamSubsystem();
}

