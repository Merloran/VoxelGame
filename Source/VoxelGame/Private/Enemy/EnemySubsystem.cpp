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
	Collection.InitializeDependency<UMassRepresentationSubsystem>();
	Collection.InitializeDependency<UMassEntitySubsystem>();
	Collection.InitializeDependency<UMassActorSubsystem>();

	Super::Initialize(Collection);


	UWorld* World = GetWorld();
	EntitySystem = World->GetSubsystem<UMassEntitySubsystem>();
	EntityManager = &EntitySystem->GetMutableEntityManager();
	RepresentationSubsystem = World->GetSubsystem<UMassRepresentationSubsystem>();
	//ActorSubsystem = World->GetSubsystem<UMassActorSubsystem>();


	RatTemplateIndex = RegisterEnemyActorTemplate(TEXT("/Game/Enemy/BP_Enemy.BP_Enemy_C"));
	//TSubclassOf<AActor> RatBPClass = LoadObject<UClass>(nullptr, TEXT("/Game/Enemy/BP_Enemy.BP_Enemy_C"));
	//RatTemplateIndex = RepresentationSubsystem->FindOrAddTemplateActor(RatBPClass);
}

bool UEnemySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);
	return World && World->IsGameWorld();
}

int16 UEnemySubsystem::RegisterEnemyActorTemplate(const TCHAR* ActorBluePrintPath)
{
	// Load BP actor class (replace path with your BP path)
	TSubclassOf<AActor> EnemyBPClass = LoadObject<UClass>(nullptr, ActorBluePrintPath);
	//TSubclassOf<AActor> EnemyBPClass = LoadObject<UClass>(nullptr, TEXT("/Game/Enemy/BP_Enemy.BP_Enemy_C"));
	if (!EnemyBPClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ENEMY-SYSTEM EnemySubsystem: couldn't load enemy BP class"));
		return -1;
	}

	int16 TemplateIndex = RepresentationSubsystem->FindOrAddTemplateActor(EnemyBPClass);
	UE_LOG(LogTemp, Log, TEXT("ENEMY-SYSTEM EnemySubsystem: Registered enemy template index %d"), RatTemplateIndex);
	return TemplateIndex;
}


/**/
void UEnemySubsystem::SpawnEnemies() 
{
	UE_LOG(LogTemp, Log, TEXT("ENEMY-SYSTEM SpawnEnemies"));
	UWorld* World = GetWorld(); 
	if (!World) return; 
	//UMassEntitySubsystem* EntitySystem = World->GetSubsystem<UMassEntitySubsystem>();
	//FMassEntityManager& EntityManager = EntitySystem->GetMutableEntityManager(); 
	//UMassRepresentationSubsystem* RepresentationSubsystem = World->GetSubsystem<UMassRepresentationSubsystem>(); 
	UMassActorSubsystem* ActorSubsystem = World->GetSubsystem<UMassActorSubsystem>(); 
	//if (!RepresentationSubsystem)
	//{ 
	//	UE_LOG(LogTemp, Warning, TEXT("RegisterEnemyMeshVisualization: MassRepresentationSubsystem missing")); 
	//	return;
	//} 
	 
	EnemyArchetype = EntityManager->CreateArchetype({ 
		FTransformFragment::StaticStruct(),
		FEnemyFragment::StaticStruct(),
		FMassRepresentationFragment::StaticStruct(),
		FMassRepresentationLODFragment::StaticStruct(), 
		FMassActorFragment::StaticStruct(),
		FHealthFragment::StaticStruct() 
	}); 

	for (int32 i = 0; i < 20; i++)
	{
		FMassEntityHandle Entity = EntityManager->CreateEntity(EnemyArchetype); 
		//Transform 
		FTransformFragment& Transform = EntityManager->GetFragmentDataChecked<FTransformFragment>(Entity); 
		Transform.GetMutableTransform().SetLocation(FVector(FMath::RandRange(-2000, 2000), FMath::RandRange(-2000, 2000), 0));
		// Enemy logic 
		FEnemyFragment& Enemy = EntityManager->GetFragmentDataChecked<FEnemyFragment>(Entity); 
		Enemy.Id = i; 
		Enemy.TargetPosition = FVector::ZeroVector + FVector(0, 0, 0); 
		Enemy.MoveSpeed = 100.f; 
		Enemy.AttackRange = 150.f;
		Enemy.Damage = 1.f;
		Enemy.AttackCooldown = 1;
		// Mass representation fragment (tells Mass how to visualize)
		FMassRepresentationFragment& Representation = EntityManager->GetFragmentDataChecked<FMassRepresentationFragment>(Entity); 
		//Representation.CurrentRepresentation = EMassRepresentationType::StaticMeshInstance; 
		Representation.CurrentRepresentation = EMassRepresentationType::HighResSpawnedActor;
		Representation.PrevRepresentation = EMassRepresentationType::None; 
		//Representation.StaticMeshDescHandle = RatHandle; // IMPORTANT 
		Representation.HighResTemplateActorIndex = RatTemplateIndex; 
		Representation.PrevTransform = Transform.GetTransform(); 
		Representation.PrevLODSignificance = -1.0f;
		
		// LOD fragment (force it visible so instances are added right away)
		FMassRepresentationLODFragment& LODFragment = EntityManager->GetFragmentDataChecked<FMassRepresentationLODFragment>(Entity); 
		LODFragment.LOD = EMassLOD::High; 
		LODFragment.PrevLOD = EMassLOD::Max; 
		LODFragment.PrevVisibility = EMassVisibility::Max; 
		LODFragment.LODSignificance = 0.0f; 


		FMassActorFragment& ActorFragment = EntityManager->GetFragmentDataChecked<FMassActorFragment>(Entity); 
		if (!ActorFragment.IsValid()) // check if actor already exists 
		{ 
			FMassActorPostSpawnDelegate PostSpawnDelegate; 
			PostSpawnDelegate.BindLambda([ActorSubsystem, &ActorFragment](const FMassActorSpawnRequestHandle& RequestHandle, FConstStructView SpawnRequestView) { 
				// Try to access the spawn request data 
				if (SpawnRequestView.IsValid() && SpawnRequestView.GetScriptStruct() == FMassActorSpawnRequest::StaticStruct()) 
				{ 
					const FMassActorSpawnRequest& SpawnRequest = SpawnRequestView.Get<const FMassActorSpawnRequest>(); 
					FMassEntityHandle MassEntity = SpawnRequest.MassAgent;
					AActor* SpawnedActor = SpawnRequest.SpawnedActor; 
					if (SpawnedActor) 
					{
						UE_LOG(LogTemp, Warning, TEXT("ENEMY-SYSTEM Spawned Actor!"));
						ActorSubsystem->SetHandleForActor(SpawnRequest.SpawnedActor, SpawnRequest.MassAgent);
						ActorFragment.SetAndUpdateHandleMap(MassEntity, SpawnedActor, true); 
						SpawnedActor->FindComponentByClass<UEnemyComponent>()->Entity = MassEntity;
					}
					UE_LOG(LogTemp, Warning, TEXT("ENEMY-SYSTEM Spawned Actor finish!"));
				} 
				else
				{ 
					UE_LOG(LogTemp, Warning, TEXT("ENEMY-SYSTEM Unexpected struct type in SpawnRequestView!")); 
				} 
				return EMassActorSpawnRequestAction::Keep; 
				});
			AActor* SpawnedActor = RepresentationSubsystem->GetOrSpawnActorFromTemplate( 
				Entity, 
				Transform.GetTransform(),
				RatTemplateIndex, 
				Representation.ActorSpawnRequestHandle,
				MAX_FLT, 
				FMassActorPreSpawnDelegate(),
				PostSpawnDelegate);
		} 
	} 
}

void UEnemySubsystem::DamageEnemy(UEnemyComponent* Enemy, float Damage)
{
	const FMassEntityHandle& Entity = Enemy->Entity;
	FHealthFragment& HealthFragment = EntityManager->GetFragmentDataChecked<FHealthFragment>(Entity);
	HealthFragment.Health -= Damage;
	if (HealthFragment.Health <= 0.0f)
	{
		FMassActorFragment& ActorFrag = EntityManager->GetFragmentDataChecked<FMassActorFragment>(Entity);
		if (ActorFrag.IsValid())
		{
			if (AActor* Actor = ActorFrag.GetMutable())
			{
				Actor->Destroy();
			}
		}

		EntityManager->DestroyEntity(Entity);
	}
}

void UEnemySubsystem::DamageTarget(int32 Target, float Damage)
{
	if (TargetHealth)
	{
		TargetHealth->TakeDamage(
			nullptr,
			Damage,
			nullptr,
			FVector(),
			nullptr,
			FName(),
			FVector(),
			nullptr,
			nullptr);
	}
}

//void UEnemySubsystem::SetTargetPosition(const FVector& NewTargetPosition)
//{
//	TargetPosition = NewTargetPosition;
//	// You could also iterate all entities and update per-entity FEnemyFragment::TargetPosition here if you want immediate per-entity value change.
//	UE_LOG(LogTemp, Log, TEXT("EnemySubsystem: SetTargetPosition %s"), *TargetPosition.ToString());
//}