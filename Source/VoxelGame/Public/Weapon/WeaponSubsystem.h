// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "Engine/Texture2D.h"
#include "WeaponSubsystem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VOXELGAME_API UWeaponSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UWeaponSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UFUNCTION(BlueprintCallable, Category = "WeaponSystem")
	//UTexture2D* Initialize(UTexture2D* BurnMap);
	UTexture2D* Initialize(UTexture2D* BurnMap, int32 MapWidth, int32 MapLength, int32 MapHeight, FVector MapOffset);
	UFUNCTION(BlueprintCallable, Category = "WeaponSystem")
	void ProcessCone(FVector Direction, float Radius, FVector BeginPosition, float Angle, float Value);
	UFUNCTION(BlueprintCallable, Category = "WeaponSystem")
	void ProcessArea(float Radius, FVector ReferencePosition, float Value);
	UFUNCTION(BlueprintCallable, Category = "WeaponSystem")
	void UpdateTexture(bool bFreeData = false);

private:

	UPROPERTY()
	UTexture2D* BurnsMap;
	int32 Width;
	int32 Length;
	int32 Height;
	FVector Offset;
	
};
