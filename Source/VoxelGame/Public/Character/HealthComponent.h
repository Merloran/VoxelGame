// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, deathCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath onDeath;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath onInjure;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath onHeal;

protected:
	UPROPERTY(EditAnywhere)
	float maxHealth;

	UPROPERTY(VisibleAnywhere)
	float currentHealth;

	UPROPERTY(EditAnywhere)
	float healthRegenerationRate;

public:
	UHealthComponent();

	UFUNCTION(BlueprintCallable)
	void heal(float amount, AActor* causer = nullptr);

	UFUNCTION(BlueprintCallable)
	void injure(float amount, AActor* causer = nullptr);

	UFUNCTION(BlueprintCallable)
	void TakeDamage(AActor *DamagedActor, 
					float Damage, 
					AController *InstigatedBy, 
					FVector HitLocation, 
					UPrimitiveComponent *FHitComponent,
					FName BoneName, 
					FVector ShotFromDirection, 
					const UDamageType *DamageType, 
					AActor *DamageCauser);

	void InstantKill(AActor *DamagedActor, 
					 UPrimitiveComponent *FHitComponent, 
					 FVector ShotFromDirection,
					 const UDamageType *DamageType, 
					 AActor *DamageCauser);

	UFUNCTION(BlueprintCallable)
	float get_max_health();

	UFUNCTION(BlueprintCallable)
	float get_current_health();

	void set_current_health(float value);

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void on_zero_health(AActor* causer = nullptr) const;		
};