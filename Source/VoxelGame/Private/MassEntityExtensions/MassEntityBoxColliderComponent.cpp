// Fill out your copyright notice in the Description page of Project Settings.


#include "MassEntityExtensions/MassEntityBoxColliderComponent.h"
#include "MassEntityExtensions/MassEntityCollisionsSubsystem.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UMassEntityBoxColliderComponent::UMassEntityBoxColliderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


    // DebugBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DebugBox"));
    // DebugBox->SetBoxExtent(FVector(0.5f * Size));
    // DebugBox->SetRelativeLocation(FVector(PositionOffset));
    // DebugBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // DebugBox->SetHiddenInGame(true);
    // DebugBox->SetIsVisualizationComponent(true);
    // DebugBox->bIsEditorOnly = true;
}


void UMassEntityBoxColliderComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	UMassEntityCollisionsSubsystem* MassEntityCollisionsSubsystem = World->GetSubsystem<UMassEntityCollisionsSubsystem>();
	ColliderId = MassEntityCollisionsSubsystem->CreateBoxCollider(FVector3f(GetOwner()->GetActorLocation()) + PositionOffset, Size);
}

void UMassEntityBoxColliderComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	// UWorld* World = GetWorld();
	// UMassEntityCollisionsSubsystem* MassEntityCollisionsSubsystem = World->GetSubsystem<UMassEntityCollisionsSubsystem>();
	// MassEntityCollisionsSubsystem->DeleteBoxCollider(ColliderId);
}

#if WITH_EDITOR
void UMassEntityBoxColliderComponent::OnRegister()
{
    Super::OnRegister();

    if (!GetWorld() || GetWorld()->IsGameWorld())
        return;


    DrawDebugBox(
        GetWorld(),
        GetOwner()->GetActorLocation() + FVector(PositionOffset),
        FVector(0.5f * FVector(Size)),
        FQuat::Identity,
        FColor::Cyan,
        true
    );
}

void UMassEntityBoxColliderComponent::PostEditChangeProperty(
    FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (!GetWorld())
        return;

    FlushPersistentDebugLines(GetWorld());

    DrawDebugBox(
        GetWorld(),
        GetOwner()->GetActorLocation() + FVector(PositionOffset),
        FVector(0.5f * FVector(Size)),
        FQuat::Identity,
        FColor::Cyan,
        true
    );
}
#endif