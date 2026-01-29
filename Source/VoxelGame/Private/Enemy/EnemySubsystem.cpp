// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/EnemySubsystem.h"

#include "Enemy/EnemyComponent.h"
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

	EnemyDeathMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/MI_Swelling.MI_Swelling"));

	RatTemplateIndex = RegisterEnemyActorTemplate(TEXT("/Game/Enemy/BP_Enemy.BP_Enemy_C"));
	RatEliteTemplateIndex = RegisterEnemyActorTemplate(TEXT("/Game/Enemy/BP_Enemy_Elite.BP_Enemy_Elite_C"));
	EnemyTypes.Add(RatTemplateIndex);
	EnemyTypes.Add(RatEliteTemplateIndex);

	EnemyArchetype = EntityManager->CreateArchetype({
		FTransformFragment::StaticStruct(),
		FEnemyFragment::StaticStruct(),
		FMassRepresentationFragment::StaticStruct(),
		FMassRepresentationLODFragment::StaticStruct(),
		FMassActorFragment::StaticStruct(),
		FHealthFragment::StaticStruct()
		});
		
	// Configuring Query
	//AllEntityQuery = FMassEntityQuery(EntityManager);
	//AllEntityQuery.AddRequirement<FEnemyFragment>(EMassFragmentAccess::ReadOnly);
	////AllEntityQuery.CacheArchetypes(EntityManager);
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
	if (!EnemyBPClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ENEMY-SYSTEM EnemySubsystem: couldn't load enemy BP class"));
		return -1;
	}

	int16 TemplateIndex = RepresentationSubsystem->FindOrAddTemplateActor(EnemyBPClass);
	UE_LOG(LogTemp, Log, TEXT("ENEMY-SYSTEM EnemySubsystem: Registered enemy template index %d"), RatTemplateIndex);
	return TemplateIndex;
}

void UEnemySubsystem::SpawnEnemies(EEnemyType EnemyType, int32 Number, FEnemyData EnemyData, const FVector& MinPositionRange, const FVector& MaxPositionRange)
{
	UE_LOG(LogTemp, Log, TEXT("ENEMY-SYSTEM SpawnEnemies"));
	UWorld* World = GetWorld();
	if (!World) return;
	UMassActorSubsystem* ActorSubsystem = World->GetSubsystem<UMassActorSubsystem>();


	for (int32 i = 0; i < Number; ++i)
	{
		FMassEntityHandle Entity = EntityManager->CreateEntity(EnemyArchetype);
		//Transform 
		FTransformFragment& Transform = EntityManager->GetFragmentDataChecked<FTransformFragment>(Entity);
		Transform.GetMutableTransform().SetLocation(FVector(FMath::RandRange(MinPositionRange.X, MaxPositionRange.X), FMath::RandRange(MinPositionRange.Y, MaxPositionRange.Y), 0));
		// Enemy logic 
		FEnemyFragment& Enemy = EntityManager->GetFragmentDataChecked<FEnemyFragment>(Entity);
		//Enemy.Id = i;
		Enemy.Target = FMath::RandRange(0, Targets.Num() - 1);
		if (Enemy.Target == PlayerTargetIndex)
		{
			if (PlayerTargetIndex)
			{
				--Enemy.Target;
			}
			else
			{
				++Enemy.Target;
			}
		}
		Enemy.MoveSpeed = EnemyData.Speed;
		Enemy.PlayerDetectionRange = EnemyData.PlayerDetectionRange;
		Enemy.AttackRange = EnemyData.AttackRange;
		Enemy.Damage = EnemyData.Damage;
		Enemy.AttackCooldown = EnemyData.AttackCooldown;
		Enemy.Radius = EnemyData.SizeRadius;
		FHealthFragment& Health = EntityManager->GetFragmentDataChecked<FHealthFragment>(Entity);
		Health.MaxHealth = EnemyData.MaxHealth;
		Health.Health = EnemyData.MaxHealth;
		// Mass representation fragment (tells Mass how to visualize)
		FMassRepresentationFragment& Representation = EntityManager->GetFragmentDataChecked<FMassRepresentationFragment>(Entity);
		Representation.CurrentRepresentation = EMassRepresentationType::HighResSpawnedActor;
		Representation.PrevRepresentation = EMassRepresentationType::None;
		Representation.HighResTemplateActorIndex = EnemyTypes[(int8)EnemyType];
		Representation.PrevTransform = Transform.GetTransform();
		Representation.PrevLODSignificance = -1.0f;

		// LOD fragment (force it visible so instances are added right away)
		FMassRepresentationLODFragment& LODFragment = EntityManager->GetFragmentDataChecked<FMassRepresentationLODFragment>(Entity);
		LODFragment.LOD = EMassLOD::High;
		LODFragment.PrevLOD = EMassLOD::Max;
		LODFragment.PrevVisibility = EMassVisibility::Max;
		LODFragment.LODSignificance = 0.0f;

		int16 ActorTemplateIndex;
		switch (EnemyType) {
			case EEnemyType::Rat:
				ActorTemplateIndex = RatTemplateIndex;
				break;
			case EEnemyType::RatElite:
				ActorTemplateIndex = RatEliteTemplateIndex;
				break;
			default:
				ActorTemplateIndex = RatTemplateIndex;
				break;
		}


		FMassActorFragment& ActorFragment = EntityManager->GetFragmentDataChecked<FMassActorFragment>(Entity);
		if (!ActorFragment.IsValid()) // check if actor already exists 
		{
			FMassActorPostSpawnDelegate PostSpawnDelegate;
			PostSpawnDelegate.BindLambda([ActorSubsystem, &ActorFragment](const FMassActorSpawnRequestHandle& RequestHandle, FConstStructView SpawnRequestView) {
				if (SpawnRequestView.IsValid() && SpawnRequestView.GetScriptStruct() == FMassActorSpawnRequest::StaticStruct())
				{
					const FMassActorSpawnRequest& SpawnRequest = SpawnRequestView.Get<const FMassActorSpawnRequest>();
					FMassEntityHandle MassEntity = SpawnRequest.MassAgent;
					AActor* SpawnedActor = SpawnRequest.SpawnedActor;
					if (SpawnedActor)
					{
						ActorSubsystem->SetHandleForActor(SpawnRequest.SpawnedActor, SpawnRequest.MassAgent);
						ActorFragment.SetAndUpdateHandleMap(MassEntity, SpawnedActor, true);
						SpawnedActor->FindComponentByClass<UEnemyComponent>()->Entity = MassEntity;
					}
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
				ActorTemplateIndex,
				Representation.ActorSpawnRequestHandle,
				MAX_FLT,
				FMassActorPreSpawnDelegate(),
				PostSpawnDelegate);
		}
	}
}

float UEnemySubsystem::DamageEnemy(UEnemyComponent* Enemy, float Damage, EAttackDamageType DamageType, EAttackDamageEffect DamageEffect)
{
	const FMassEntityHandle& Entity = Enemy->Entity;
	FHealthFragment& HealthFragment = EntityManager->GetFragmentDataChecked<FHealthFragment>(Entity);
	HealthFragment.Health -= Damage;
	if (HealthFragment.Health <= 0.0f)
	{
		HealthFragment.Health = 0.0f;
		FMassActorFragment& ActorFrag = EntityManager->GetFragmentDataChecked<FMassActorFragment>(Entity);
		if (ActorFrag.IsValid())
		{
			if (AActor* Actor = ActorFrag.GetMutable())
			{
				UEnemyComponent* EnemyComponent = Actor->FindComponentByClass<UEnemyComponent>();
				if (EnemyComponent)
				{
					EnemyComponent->Die(EAttackDamageType::NORMAL);
				}
				else
				{
					Actor->Destroy();
				}
			}
		}

		EntityManager->DestroyEntity(Entity);
	}

	return HealthFragment.Health / HealthFragment.MaxHealth;
}

void UEnemySubsystem::DamageTarget(int32 Target, float Damage)
{
	if (Target < Targets.Num() && Targets[Target])
	{
		Targets[Target]->TakeDamage(
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

void UEnemySubsystem::ClearAll()
{
	//FMassExecutionContext Context(EntityManager, 0.1f, true);
	//
	//AllEntityQuery.ForEachEntityChunk(EntityManager, Context,
	//    [&](FMassExecutionContext& Ctx)
	//    {
	//		EntityManager.Defer().DestroyEntities(Ctx.GetEntities());
	//    });
	IsEnemiesCleared = true;
}