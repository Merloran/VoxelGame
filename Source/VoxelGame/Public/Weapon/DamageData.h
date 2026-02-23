// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/* UNREAL ENGINE */
#include "CoreMinimal.h"

/* WEAPON */
#include "Weapon/AttackDamageType.h"

/* GENERATED */
#include "DamageData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct VOXELGAME_API FDamageData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackDamageType DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackDamageEffect DamageEffect;
};
