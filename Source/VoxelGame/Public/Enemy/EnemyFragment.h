// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "MassCommonFragments.h"
#include "EnemyFragment.generated.h"

/*
 *
 */
USTRUCT()
struct VOXELGAME_API FEnemyFragment : public FMassFragment
{
    GENERATED_BODY()

    //UPROPERTY()
    //uint32 Id;
    UPROPERTY()
    int32 Target;
    UPROPERTY()
    float MoveSpeed = 200.f;
    UPROPERTY()
    float PlayerDetectionRange = 200.f;
    UPROPERTY()
    float AttackRange = 200.f;
    UPROPERTY()
    float Damage = 1.f;
    UPROPERTY()
    float AttackCooldown = 1.f;
    UPROPERTY()
    float AttackTimer = 0.f;
    UPROPERTY()
    float Radius = 25.f;
};

/*
 * Health fragment - holds current and max HP.
 */
USTRUCT()
struct VOXELGAME_API FHealthFragment : public FMassFragment
{
    GENERATED_BODY()

    UPROPERTY()
    float Health = 100.f;
    UPROPERTY()
    float MaxHealth = 100.f;
};