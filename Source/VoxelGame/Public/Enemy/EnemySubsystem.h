// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "UObject/Class.h"
#include "Materials/MaterialInterface.h"
#include "Subsystems/WorldSubsystem.h"

#include "Character/HealthComponent.h"

#include "Enemy/EnemyFragment.h"
#include "Enemy/EnemyComponent.h"

#include "MassEntityTypes.h"
#include "MassEntitySubsystem.h"
#include "MassEntityManager.h"
#include "MassRepresentationSubsystem.h"
#include "MassActorSubsystem.h"
#include "MassEntityQuery.h"
#include "MassEntitySubsystem.h"
//#include "MassActorSpawnRequest.h"
#include "MassRepresentationFragments.h"

#include "EnemySubsystem.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Rat        UMETA(DisplayName = "Rat"),
	RatElite   UMETA(DisplayName = "RatElite")
};

USTRUCT(BlueprintType)
struct VOXELGAME_API FEnemyData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerDetectionRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayerLoseFocusRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayerLoseFocusTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SizeRadius;
};

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
	void SpawnEnemies(EEnemyType EnemyType, int32 Number, FEnemyData EnemyData, const FVector& MinPositionRange, const FVector& MaxPositionRange);

	UFUNCTION(BlueprintCallable, Category="EnemySystem")
	float DamageEnemy(UEnemyComponent* Enemy, float Damage, EAttackDamageType DamageType, EAttackDamageEffect DamageEffect);
	UFUNCTION(BlueprintCallable, Category = "EnemySystem")
	void DamageTarget(int32 Target, float Damage);
	UFUNCTION(BlueprintCallable, Category = "EnemySystem")
	void ClearAll();
	UFUNCTION(BlueprintCallable, Category = "EnemySystem")
	int32 RegisterEnemyActorTemplate(EEnemyType EnemyType, TSubclassOf<AActor> EnemyBPClass);

	UPROPERTY(BlueprintReadWrite, Category = "EnemySystem")
	int32 PlayerTargetIndex;
	UPROPERTY(BlueprintReadWrite, Category = "EnemySystem")
	TArray<UHealthComponent*> Targets;
	UPROPERTY(BlueprintReadWrite, Category = "EnemySystem")
	TArray<FVector> TargetsPositions;
	UPROPERTY(BlueprintReadWrite, Category = "EnemySystem")
	UMaterialInterface* EnemyDeathMaterial;
	
	UPROPERTY(BlueprintReadWrite, Category = "EnemySystem")
	bool IsEnemiesCleared = false;

private:
	FMassArchetypeHandle EnemyArchetype;
	TArray<int16> EnemyTypes;


	UMassEntitySubsystem* EntitySystem;
	UMassRepresentationSubsystem* RepresentationSubsystem;
	//UMassActorSubsystem* ActorSubsystem;
	FMassEntityManager* EntityManager;

	
    FMassEntityQuery AllEntityQuery;


};
