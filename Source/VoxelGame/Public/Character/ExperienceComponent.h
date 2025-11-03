// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "ExperienceComponent.generated.h"

class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGainExperience, int64, experience);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, AActor*, pickedActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UExperienceComponent : public USphereComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Interaction")
	FOnLevelUp onLevelUp;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Interaction")
	FOnGainExperience onGainExperience;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Interaction")
	FOnPickUp OnPickUp;

protected:
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	int64 ownedExperience;

private:
	static constexpr int64 EXPERIENCE_FOR_FIRST_LEVEL = 100;
	int32 level;

public:	
	UExperienceComponent();

	UFUNCTION(BlueprintCallable)
	void gain_experience(const int64 experience);

	UFUNCTION(BlueprintCallable)
	int64 get_current_experience() const;

	UFUNCTION(BlueprintCallable)
	int32 get_level() const;

	UFUNCTION(BlueprintCallable)
	float get_experience_progress() const;

	UFUNCTION(BlueprintCallable)
	void reset_level();

protected:
	virtual void BeginPlay() override;
	
	/** Code for when something overlaps this component */
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void update_level();
};