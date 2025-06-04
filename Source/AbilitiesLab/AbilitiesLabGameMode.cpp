// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitiesLabGameMode.h"
#include "AbilitiesLabCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAbilitiesLabGameMode::AAbilitiesLabGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
