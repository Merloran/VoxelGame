// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "UObject/Class.h"
#include "Subsystems/WorldSubsystem.h"

#include "Character/HealthComponent.h"

#include "Enemy/EnemyFragment.h"
#include "Enemy/EnemyComponent.h"

#include "MassEntityTypes.h"
#include "MassEntitySubsystem.h"
#include "MassEntityManager.h"
#include "MassRepresentationSubsystem.h"
#include "MassActorSubsystem.h"
//#include "MassActorSpawnRequest.h"
#include "MassRepresentationFragments.h"

#include "EnemySubsystem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VOXELGAME_API UEnemySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	UFUNCTION(BlueprintCallable, Category = "EnemySystem")
	void SpawnEnemies();

	UFUNCTION(BlueprintCallable, Category="EnemySystem")
	void DamageEnemy(UEnemyComponent* Enemy, float Damage);
	UFUNCTION(BlueprintCallable, Category = "EnemySystem")
	void DamageTarget(int32 Target, float Damage);

	UPROPERTY(BlueprintReadWrite, Category = "EnemySystem")
	FVector TargetPosition;
	UPROPERTY(BlueprintReadWrite, Category = "EnemySystem")
	UHealthComponent* TargetHealth;

private:
	int16 RegisterEnemyActorTemplate(const TCHAR* ActorBluePrintPath);
	FMassArchetypeHandle EnemyArchetype;
	int16 RatTemplateIndex;

	UMassEntitySubsystem* EntitySystem;
	UMassRepresentationSubsystem* RepresentationSubsystem;
	//UMassActorSubsystem* ActorSubsystem;
	FMassEntityManager* EntityManager;
};
