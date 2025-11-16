// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/EnemySubsystem.h"

#include "Enemy/EnemyFragment.h"
#include "Engine/World.h"
#include "MassCommonFragments.h"
#include "MassEntitySubsystem.h"
#include "MassRepresentationSubsystem.h"
#include "MassRepresentationTypes.h"

void UEnemySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

	UWorld* World = GetWorld();
	UMassRepresentationSubsystem* RepSubsystem = World->GetSubsystem<UMassRepresentationSubsystem>();
	
	/* RAT */
	FStaticMeshInstanceVisualizationDesc RatDesc;
	UStaticMesh* RatMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Enemy/Meshes/SM_Rat.SM_Rat"));
	RatDesc.Mesh = RatMesh;
	RatDesc.MaterialOverrides.Add(GruntMaterial);
	RatDesc.bCastShadows = true;
	RatDesc.bReceivesDecals = true;
	
	RepSubsystem->FindOrAddStaticMeshDesc(RatDesc, TEXT("Rat"));

	/* ELITE RAT */
	/*
	FStaticMeshInstanceVisualizationDesc EliteDesc;
	UStaticMesh* EliteMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Enemy/Meshes/SM_Rat.SM_Rat"));
	EliteDesc.Mesh = EliteMesh;
	EliteDesc.MaterialOverrides.Add(EliteMaterial);
	/**/
}