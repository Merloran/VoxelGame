// Copyright Epic Games, Inc. All Rights Reserved.

#include "VoxelGameGameMode.h"
#include "Character/VoxelGameCharacter.h"
#include "Enemy/EnemySubsystem.h"
#include "UObject/ConstructorHelpers.h"

AVoxelGameGameMode::AVoxelGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AVoxelGameGameMode::BeginPlay()
{
	UEnemySubsystem* EnemySubsystem = GetWorld()->GetSubsystem<UEnemySubsystem>();
	UE_LOG(LogTemp, Log, TEXT("ENEMY-SYSTEM BP1"));
	if (EnemySubsystem)
	{
		UE_LOG(LogTemp, Log, TEXT("ENEMY-SYSTEM BP2"));
		EnemySubsystem->SpawnEnemies();
	}
	UE_LOG(LogTemp, Log, TEXT("ENEMY-SYSTEM BP3"));
}