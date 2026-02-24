// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "EnemyStatusConditionProcessor.generated.h"

/**
 * 
 */
UCLASS()
class VOXELGAME_API UEnemyStatusConditionProcessor : public UMassProcessor
{
	GENERATED_BODY()
	
public:
    UEnemyStatusConditionProcessor();

protected:
    virtual void ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager) override;
    virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

    FMassEntityQuery EntityQuery;
};
