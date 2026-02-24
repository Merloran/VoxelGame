// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/* CORE COMPONENTS */
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"

#include "Sound/SoundAttenuation.h"

/* MASS ENTITY */
#include "MassEntitySubsystem.h"

/* WEAPON */
#include "Weapon/AttackDamageType.h"
#include "Weapon/DamageData.h"


#include "EnemyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, FDamageData, DamageData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, RemainingHealthRatio);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOXELGAME_API UEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	USoundBase* EnemyAttackSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	USoundAttenuation* SoundAttenuation;


	// Sets default values for this component's properties
	UEnemyComponent();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnEnemyHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDeath OnEnemyDeath;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttack OnAttack;

	FMassEntityHandle Entity;
	
	void CallOnTakenDamage(float RemainingHealthRatio);
	void Die(EAttackDamageType DamageType, EAttackDamageEffect DamageEffect);
	void PerformAttack();

protected:
	
	UPROPERTY()
	UAudioComponent* EnemyAttackAudioComponent;

	void BeginPlay();

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
