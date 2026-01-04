// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Colliders.generated.h"


/**
 * 
 */
USTRUCT()
struct VOXELGAME_API FBoxCollider
{
	GENERATED_BODY()
	int id;
	bool active;
	float front;
	float back;
	float right;
	float left;
	float up;
	float down;
};
