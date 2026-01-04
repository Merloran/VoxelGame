// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "MassEntityBoxColliderComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOXELGAME_API UMassEntityBoxColliderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMassEntityBoxColliderComponent();

	UPROPERTY(EditAnywhere, Category = "Collider")
	FVector3f Size = FVector3f(1000.0f, 1000.0f, 1000.0f);
	UPROPERTY(EditAnywhere, Category = "Collider")
	FVector3f PositionOffset;

#if WITH_EDITOR
	virtual void OnRegister() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
protected:
	virtual void BeginPlay() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

private:
	int ColliderId;
	// UBoxComponent* DebugBox;
};
