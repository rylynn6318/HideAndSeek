// Copyright Epic Games, Inc. All Rights Reserved.

#include "HSTeamInfoBase.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Teams/HSTeamSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HSTeamInfoBase)

class FLifetimeProperty;

AHSTeamInfoBase::AHSTeamInfoBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TeamId(INDEX_NONE)
{
	bReplicates = true;
	bAlwaysRelevant = true;
	NetPriority = 3.0f;
	SetReplicatingMovement(false);
}

void AHSTeamInfoBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TeamTags);
	DOREPLIFETIME_CONDITION(ThisClass, TeamId, COND_InitialOnly);
}

void AHSTeamInfoBase::BeginPlay()
{
	Super::BeginPlay();

	TryRegisterWithTeamSubsystem();
}

void AHSTeamInfoBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (TeamId != INDEX_NONE)
	{
		UHSTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UHSTeamSubsystem>();
		if (TeamSubsystem)
		{
			// EndPlay can happen at weird times where the subsystem has already been destroyed
			TeamSubsystem->UnregisterTeamInfo(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void AHSTeamInfoBase::RegisterWithTeamSubsystem(UHSTeamSubsystem* Subsystem)
{
	Subsystem->RegisterTeamInfo(this);
}

void AHSTeamInfoBase::TryRegisterWithTeamSubsystem()
{
	if (TeamId != INDEX_NONE)
	{
		UHSTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UHSTeamSubsystem>();
		if (ensure(TeamSubsystem))
		{
			RegisterWithTeamSubsystem(TeamSubsystem);
		}
	}
}

void AHSTeamInfoBase::SetTeamId(int32 NewTeamId)
{
	check(HasAuthority());
	check(TeamId == INDEX_NONE);
	check(NewTeamId != INDEX_NONE);

	TeamId = NewTeamId;

	TryRegisterWithTeamSubsystem();
}

void AHSTeamInfoBase::OnRep_TeamId()
{
	TryRegisterWithTeamSubsystem();
}

