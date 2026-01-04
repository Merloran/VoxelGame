// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MassEntityExtensions/Colliders.h"
#include "MassEntityCollisionsSubsystem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VOXELGAME_API UMassEntityCollisionsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Collisions")
	int CreateBoxCollider(FVector3f Position, FVector3f Size);
	UFUNCTION(BlueprintCallable, Category = "Collisions")
	void DeleteBoxCollider(int Id);
	UFUNCTION(BlueprintCallable, Category = "Collisions")
	void SetColliderActive(int Id, bool Active);
	TArrayView<FBoxCollider> GetBoxColliders();

private:
	int _NewBoxColliderIndex = 0;
	TArray<FBoxCollider> _BoxColliders;
};
