// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "VG_PC_Base.generated.h"

/**
 * 
 */
UCLASS()
class VOXELGAME_API AVG_PC_Base : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* UsedInputMappingContext;
};
