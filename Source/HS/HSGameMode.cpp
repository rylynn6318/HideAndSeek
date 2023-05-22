// Copyright Epic Games, Inc. All Rights Reserved.

#include "HSGameMode.h"
#include "HSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHSGameMode::AHSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
