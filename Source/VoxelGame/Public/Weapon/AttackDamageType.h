// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAttackDamageType : uint8
{	
    None        	UMETA(DisplayName = "None"),
    NORMAL          UMETA(DisplayName = "NORMAL"),
    EXPLOADING      UMETA(DisplayName = "EXPLOADING"),
    FIRE            UMETA(DisplayName = "FIRE"),
    ELECTRIC        UMETA(DisplayName = "ELECTRIC"),
    POISON          UMETA(DisplayName = "POISON"),
    DEBUFF          UMETA(DisplayName = "DEBUFF"),
    NON_DAMAGING    UMETA(DisplayName = "NON_DAMAGING")
};

UENUM(BlueprintType)
enum class EAttackDamageEffect : uint8
{
    NONE            UMETA(DisplayName = "NONE"),
    BURN            UMETA(DisplayName = "BURN"),
    POISON          UMETA(DisplayName = "POISON"),
    ELECTROCUTION   UMETA(DisplayName = "ELECTROCUTION"),
    SLEEP           UMETA(DisplayName = "SLEEP"),
    CONFUSION       UMETA(DisplayName = "CONFUSION"),
    KNOCKBACK       UMETA(DisplayName = "KNOCKBACK"),
    STUN            UMETA(DisplayName = "STUN")
};