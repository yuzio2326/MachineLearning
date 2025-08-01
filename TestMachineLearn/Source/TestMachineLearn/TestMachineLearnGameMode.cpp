// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestMachineLearnGameMode.h"
#include "TestMachineLearnCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestMachineLearnGameMode::ATestMachineLearnGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
