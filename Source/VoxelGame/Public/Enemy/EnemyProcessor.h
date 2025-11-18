// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MassEntityTypes.h"
#include "Enemy/EnemyFragment.h"
#include "EnemyProcessor.generated.h"

UCLASS()
class VOXELGAME_API UEnemyProcessor : public UMassProcessor
{
	GENERATED_BODY()

public:
    UEnemyProcessor();

protected:
    virtual void ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager) override;
    virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

    FMassEntityQuery EntityQuery;
};
