// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineCamera.h"

#include "DrawDebugHelpers.h"
#include "Components/SplineComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogCameraSpline);

ASplineCamera::ASplineCamera()
	: LookAtTransitionValue(0.1f)
	, CameraTransitionValue(0.1f)
	, bIsDebugDraw(false)
	, Time(-1.0f)
	, Size(10.0f)
	, PreviousLookAtPoint(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASplineCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!CameraSpline || !LookAtSpline || !Player.IsValid())
	{
		return;
	}

	USplineComponent *CameraSplineComponent = CameraSpline->GetComponentByClass<USplineComponent>();
	USplineComponent *LookAtSplineComponent = LookAtSpline->GetComponentByClass<USplineComponent>();
	if (!CameraSplineComponent || !LookAtSplineComponent)
	{
		UE_LOG(LogCameraSpline, Error, TEXT("'%s' Splines for camera are not specified, cannot handle camera logic!"), *GetNameSafe(this));
		return;
	}

	FVector PlayerPosition = Player->GetActorLocation();
	FVector CameraPosition = GetActorLocation();
	FVector CameraSplinePosition = CameraSplineComponent->FindLocationClosestToWorldLocation(PlayerPosition,
																							 ESplineCoordinateSpace::World);
	FVector NewLocation = FMath::Lerp(CameraPosition, CameraSplinePosition, CameraTransitionValue);

	FVector LookAtSplinePosition = LookAtSplineComponent->FindLocationClosestToWorldLocation(PlayerPosition, 
																							 ESplineCoordinateSpace::World);
	PreviousLookAtPoint = FMath::Lerp(PreviousLookAtPoint, LookAtSplinePosition, LookAtTransitionValue);
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(CameraPosition, PreviousLookAtPoint);

	SetActorLocationAndRotation(NewLocation, NewRotation);

#if WITH_EDITOR
	if (bIsDebugDraw)
	{
		DrawDebugPoint(GetWorld(), 
					   CameraSplinePosition, 
					   Size, 
					   CameraSplineComponent->EditorUnselectedSplineSegmentColor.ToFColor(true), 
					   false,
					   Time);
		DrawDebugPoint(GetWorld(), 
					   LookAtSplinePosition, 
					   Size, 
					   LookAtSplineComponent->EditorUnselectedSplineSegmentColor.ToFColor(true),
					   false, 
					   Time);
	}
#endif
}

void ASplineCamera::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}
