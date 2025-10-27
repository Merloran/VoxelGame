// Copyright Epic Games, Inc. All Rights Reserved.

#include "VoxelGameGameMode.h"
#include "VoxelGameCharacter.h"
#include "VG_PC_ThirdPersonController.h"
#include "UObject/ConstructorHelpers.h"

AVoxelGameGameMode::AVoxelGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	};
}
