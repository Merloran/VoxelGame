// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "MassCommonFragments.h"
#include "MassRepresentationFragments.h"
#include "EnemyFragment.generated.h"

/*
 *
 */
USTRUCT()
struct VOXELGAME_API FEnemyFragment : public FMassFragment
{
    GENERATED_BODY()

    UPROPERTY()
    uint32 Eid;
    UPROPERTY()
    FVector TargetPosition = FVector::ZeroVector;
    UPROPERTY()
    float MoveSpeed = 200.f;
    UPROPERTY()
    float AttackRange = 200.f;
};


/*
 *
 */
USTRUCT()
struct FEnemyStateFragment : public FMassFragment
{
    GENERATED_BODY()

    UPROPERTY()
    uint8 bIsAttacking : 1;

    UPROPERTY()
    uint8 bIsMoving : 1;
};

/*
 *
 */
USTRUCT()
struct FEnemyRepresentationFragment : public FMassFragment
{
    GENERATED_BODY()

    // Index or ID for which visual template to use
    int32 VisualType = 0;
};

/*
 * Health fragment - holds current and max HP.
 */
USTRUCT()
struct FHealthFragment : public FMassFragment
{
    GENERATED_BODY()

    float Health = 100.f;
    float MaxHealth = 100.f;
};

/*
 * Damage fragment - transient fragment used when something takes a hit.
 */
USTRUCT()
struct FDamageFragment : public FMassFragment
{
    GENERATED_BODY()

    float DamageAmount = 0.f;
};