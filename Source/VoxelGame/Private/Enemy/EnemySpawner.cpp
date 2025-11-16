// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemySpawner.h"

#include "MassRepresentationSubsystem.h"
#include "MassRepresentationTypes.h"

////void UEnemyVisualizationSubsystem::InitializeRepresentationActors(FSubsystemCollectionBase& Collection)
//void InitializeRepresentationActorsBP()
//{
//    //Super::Initialize(Collection);
//
//    UWorld* World = GetWorld();
//    UMassRepresentationSubsystem* RepSubsystem = World->GetSubsystem<UMassRepresentationSubsystem>();
//
//    // Register an Actor template (can be Pawn or BP)
//    FSoftObjectPath GruntPath(TEXT("/Game/Enemies/BP_EnemyGrunt.BP_EnemyGrunt_C"));
//    RepSubsystem->RegisterActorClassTemplate(GruntPath, FName("GruntTemplate"));
//
//    FSoftObjectPath ElitePath(TEXT("/Game/Enemies/BP_EnemyElite.BP_EnemyElite_C"));
//    RepSubsystem->RegisterActorClassTemplate(ElitePath, FName("EliteTemplate"));
//}
//
//void RegisterEnemyRepresentation(UWorld* World, TSubclassOf<AActor> VisualTemplate)
//{
//    UMassRepresentationSubsystem* RepSubsystem = World->GetSubsystem<UMassRepresentationSubsystem>();
//    if (!RepSubsystem) return;
//
//    // Register a representation type
//    FStaticMeshInstanceVisualizationDesc Desc;
//    Desc.TemplateActor = VisualTemplate;
//    Desc.VisualType = (uint8)0; // match FEnemyRepresentationFragment.VisualType
//
//    RepSubsystem->RegisterRepresentation(Desc);
//}
//
//
//
//void RegisterEnemyRepresentation(UWorld* World)
//{
//    Super::Initialize(Collection);
//
//    //UWorld* World = GetWorld();
//    UMassRepresentationSubsystem* RepSubsystem = World->GetSubsystem<UMassRepresentationSubsystem>();
//
//    // --- Rat mesh ---
//    FStaticMeshInstanceVisualizationDesc GruntDesc;
//    //GruntDesc.Mesh = GruntMesh; // assign UStaticMesh*
//    GruntDesc.MaterialOverrides.Add(GruntMaterial);
//    GruntDesc.bCastShadows = true;
//    GruntDesc.bReceivesDecals = true;
//
//    RepSubsystem->FindOrAddStaticMeshDesc(GruntDesc, TEXT("Rat"));
//
//    // --- EliteRat mesh ---
//    FStaticMeshInstanceVisualizationDesc EliteDesc;
//    //EliteDesc.Mesh = EliteMesh;
//    EliteDesc.MaterialOverrides.Add(EliteMaterial);
//
//    RepSubsystem->RegisterStaticMeshInstanceVisualization(EliteDesc, TEXT("EliteRat"));
//}


void SpawnEnemies(UWorld* World)
{
    if (!World) return;

    //RegisterEnemyRepresentation(World);

    UMassEntitySubsystem* EntitySystem = World->GetSubsystem<UMassEntitySubsystem>();
    if (!EntitySystem) return;

    FMassEntityManager& EntityManager = EntitySystem->GetMutableEntityManager();

    // Define what fragments each enemy has
    FMassArchetypeHandle EnemyArchetype = EntityManager.CreateArchetype({ 
        FTransformFragment::StaticStruct(),
        FEnemyFragment::StaticStruct(),
        FEnemyRepresentationFragment::StaticStruct(),
        FMassRepresentationFragment::StaticStruct()
    });

    for (int32 i = 0; i < 5; i++)
    {
        FMassEntityHandle Entity = EntityManager.CreateEntity(EnemyArchetype);

        FVector SpawnPos = FVector(FMath::RandRange(-2000, 2000), FMath::RandRange(-2000, 2000), 50);
        FTransformFragment& Transform = EntityManager.GetFragmentDataChecked<FTransformFragment>(Entity);
        Transform.GetMutableTransform().SetLocation(SpawnPos);

        /* MOVEMENT */
        FEnemyFragment& Enemy = EntityManager.GetFragmentDataChecked<FEnemyFragment>(Entity);
        Enemy.Eid = i; // all enemies move toward center
        Enemy.TargetPosition = FVector(0, 0, 50); // all enemies move toward center
        Enemy.MoveSpeed = 50.f;
        Enemy.AttackRange = 150.f;
        UE_LOG(LogTemp, Display, TEXT("This is a display message index: %d"), i);

        /* REPRESENTATION */
        // For BP
        /*
        FMassRepresentationFragment& RepFragment = EntityManager.GetFragmentDataChecked<FMassRepresentationFragment>(Entity);
        RepFragment.CurrentRepresentation = EMassRepresentationType::HighResSpawnedActor;
        RepFragment.ActorTemplateID = FName("GruntTemplate");
        /**/

        // For Static Mess
        //FEnemyRepresentationFragment& EnemyRep = EntityManager.GetFragmentDataChecked<FEnemyRepresentationFragment>(Entity);
        //EnemyRep.VisualType = 0; // e.g. grunt
        //
        //auto& RepFragment = EntityManager.GetFragmentDataChecked<FMassRepresentationFragment>(Entity);
        //RepFragment.CurrentRepresentation = EMassRepresentationType::StaticMeshInstance;
        //
        //// Pick the right descriptor based on enemy type
        //switch (EnemyRep.VisualType)
        //{
        //case 0:
        //    RepFragment.StaticMeshDescHandle = FName("Rat");
        //    break;
        //case 1:
        //    RepFragment.StaticMeshDescHandle = FName("EliteRat");
        //    break;
        //}

    }
}