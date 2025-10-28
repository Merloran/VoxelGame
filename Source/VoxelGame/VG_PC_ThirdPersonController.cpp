// Fill out your copyright notice in the Description page of Project Settings.


#include "VG_PC_ThirdPersonController.h"
#include "EnhancedInputSubsystems.h"

AVG_PC_ThirdPersonController::AVG_PC_ThirdPersonController()
{
    ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCRef(
        TEXT("/Game/ThirdPerson/Input/IMC_ThirdPerson")
    );

    if (IMCRef.Succeeded())
    {
        UsedInputMappingContext = IMCRef.Object;
        UE_LOG(LogTemp, Log, TEXT("Loaded IMC from path: %s"), *UsedInputMappingContext->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find IMC asset!"));
    }
}

void AVG_PC_ThirdPersonController::BeginPlay()
{
    Super::BeginPlay();

    ULocalPlayer* localPlayer = GetLocalPlayer();
    if (UsedInputMappingContext && localPlayer)
    {
        UEnhancedInputLocalPlayerSubsystem* inputSubsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
        if (inputSubsystem)
        {
            inputSubsystem->ClearAllMappings();
            inputSubsystem->AddMappingContext(UsedInputMappingContext, 0);
        }
    }
}