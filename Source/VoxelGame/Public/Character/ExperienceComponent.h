// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "ExperienceComponent.generated.h"

class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGainExperience, int64, Experience);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, AActor*, PickedActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UExperienceComponent : public USphereComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Interaction")
	FOnLevelUp OnLevelUp;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Interaction")
	FOnGainExperience OnGainExperience;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Interaction")
	FOnPickUp OnPickUp;

protected:
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	int64 OwnedExperience;

private:
	static constexpr int64 EXPERIENCE_FOR_FIRST_LEVEL = 100;
	int32 Level;

public: 
	UExperienceComponent();

	UFUNCTION(BlueprintCallable)
	void GainExperience(const int64 Experience);

	UFUNCTION(BlueprintCallable)
	int64 GetCurrentExperience() const;

	UFUNCTION(BlueprintCallable)
	int32 GetLevel() const;

	UFUNCTION(BlueprintCallable)
	float GetExperienceProgress() const;

	UFUNCTION(BlueprintCallable)
	void ResetLevel();

protected:
	virtual void BeginPlay() override;

	/** Code for when something overlaps this component */
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
                              AActor* OtherActor, 
                              UPrimitiveComponent* OtherComp, 
                              int32 OtherBodyIndex, 
                              bool bFromSweep, 
                              const FHitResult& SweepResult);

private:
	void UpdateLevel();
};