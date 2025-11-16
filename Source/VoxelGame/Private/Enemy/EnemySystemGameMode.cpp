// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemySystemGameMode.h"
#include "Enemy/EnemySpawner.h"


void AEnemySystemGameMode::BeginPlay()
{
    Super::BeginPlay();
    SpawnEnemies(GetWorld());
}