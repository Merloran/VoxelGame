// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "SplineCamera.generated.h"

class ACharacter;
class USplineComponent;
DECLARE_LOG_CATEGORY_EXTERN(LogCameraSpline, Log, All);

UCLASS()
class ASplineCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	ASplineCamera();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	/** Spline that camera is moving on */
	UPROPERTY(EditAnywhere, Category="Spline")
	TObjectPtr<AActor> CameraSpline;

	/** Spline that camera is looking at */
	UPROPERTY(EditAnywhere, Category = "Spline")
	TObjectPtr<AActor> LookAtSpline;

	/** Value that defines how smooth camera is change look at point */
	UPROPERTY(EditAnywhere, Category = "Spline")
	float LookAtTransitionValue;

	/** Value that defines how smooth camera is change location */
	UPROPERTY(EditAnywhere, Category = "Spline")
	float CameraTransitionValue;

	/** Turn on debug points on spline to know where camera looking and where camera is on other spline */
	UPROPERTY(EditAnywhere, Category = "Spline|Debug")
	bool bIsDebugDraw;

	/** Time of displaying points on splines */
	UPROPERTY(EditAnywhere, Category = "Spline|Debug")
	float Time;

	/** Size of displayed points on splines */
	UPROPERTY(EditAnywhere, Category = "Spline|Debug")
	float Size;

private:
	TWeakObjectPtr<ACharacter> Player;

	FVector PreviousLookAtPoint;
};
