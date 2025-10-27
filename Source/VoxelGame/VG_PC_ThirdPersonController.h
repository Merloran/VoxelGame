// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "VG_PC_ThirdPersonController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VOXELGAME_API AVG_PC_ThirdPersonController : public APlayerController
{
	GENERATED_BODY()

public:
	AVG_PC_ThirdPersonController();
	virtual void BeginPlay() override;

private:
	UInputMappingContext* UsedInputMappingContext = nullptr;
};
