// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "WeaponProcessor.generated.h"

/**
 * 
 */
UCLASS()
class VOXELGAME_API UWeaponProcessor : public UMassProcessor
{
	GENERATED_BODY()
	
public:
    UWeaponProcessor();

protected:
    virtual void ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager) override;
    virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

    FMassEntityQuery EntityQuery;
};
