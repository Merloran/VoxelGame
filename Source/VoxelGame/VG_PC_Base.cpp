// Fill out your copyright notice in the Description page of Project Settings.

#include "VG_PC_Base.h"
#include "EnhancedInputSubsystems.h"

void AVG_PC_Base::BeginPlay()
{
    Super::BeginPlay();

    // Apply input mapping context
    if (ULocalPlayer* LP = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsys = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            Subsys->ClearAllMappings();
            Subsys->AddMappingContext(UsedInputMappingContext, 0);
        }
    }
}