// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeathCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath OnInjure;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeath OnHeal;

protected:
	UPROPERTY(EditAnywhere)
	float MaxHealth;

	UPROPERTY(VisibleAnywhere)
	float CurrentHealth;

	UPROPERTY(EditAnywhere)
	float HealthRegenerationRate;

public:
	UHealthComponent();

	UFUNCTION(BlueprintCallable)
	void Heal(float Amount, AActor* Causer = nullptr);

	UFUNCTION(BlueprintCallable)
	void Injure(float Amount, AActor* Causer = nullptr);

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
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();

	void SetCurrentHealth(float Value);

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnZeroHealth(AActor* Causer = nullptr) const;
};