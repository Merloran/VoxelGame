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
    int32 Target = -1;
    UPROPERTY()
    int32 LastTarget = -1;
    UPROPERTY()
    float MoveSpeed = 200.f;
    UPROPERTY()
    float PlayerDetectionRange = 200.f;
    UPROPERTY()
    float PlayerLoseFocusRange = 400.f;
    UPROPERTY()
    float PlayerLoseFocusTime = 10.f;
    UPROPERTY()
    float PlayerLoseFocusTimer = 0.f;
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

/*
 *
 */
USTRUCT()
struct VOXELGAME_API FEnemyStatusCondition : public FMassFragment
{
    GENERATED_BODY()

    /* KNOCKBACK DATA */
    FVector KnockbackDirection = FVector();
    float KnockbackVelocity = 0.f;
    float KnockbackDecceleration = 0.f;

    /* SLOWDOWN */
    float SlowdownFactor = 0.f;
    float SlowdownTimer = 0.f;
    float SlowdownCooldownTimer = 0.f;

    /* STUN */
    float StunTimer = 0.f;
    float StunCooldownTimer = 0.f;

    /* BURN */
    float BurnDamage = 0.f;
    float BurnTickTimer = 0.f;
    float BurnTimer = 0.f;

    /* POISON */
    float PoisonDamage = 0.f;
    float PoisonTickTimer = 0.f;
    float PoisonTimer = 0.f;
};
