// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAM415ProjectGameMode.h"
#include "GAM415ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAM415ProjectGameMode::AGAM415ProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
