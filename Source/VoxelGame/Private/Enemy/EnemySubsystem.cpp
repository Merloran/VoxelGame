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

	EnemyArchetype = EntityManager->CreateArchetype({
		FTransformFragment::StaticStruct(),
		FEnemyFragment::StaticStruct(),
		FEnemyStatusCondition::StaticStruct(),
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

int32 UEnemySubsystem::RegisterEnemyActorTemplate(EEnemyType EnemyType, TSubclassOf<AActor> EnemyBPClass)
{
	if (!EnemyBPClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ENEMY-SYSTEM Provided during registration Actor Blueprint class is null"));
		return -1;
	}

	int16 TemplateIndex = RepresentationSubsystem->FindOrAddTemplateActor(EnemyBPClass);
	UE_LOG(LogTemp, Log, TEXT("ENEMY-SYSTEM EnemySubsystem: Registered enemy template index %d"), TemplateIndex);

	if (EnemyTypes.Num() <= (int32)EnemyType)
	{
		EnemyTypes.SetNum((int32)EnemyType + 1);
	}

	EnemyTypes[(int32)EnemyType] = TemplateIndex;

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
		Enemy.PlayerLoseFocusRange = EnemyData.PlayerLoseFocusRange;
		Enemy.PlayerLoseFocusTime = EnemyData.PlayerLoseFocusTime;
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
				ActorTemplateIndex = EnemyTypes[(int32)EEnemyType::Rat];
				break;
			case EEnemyType::RatElite:
				ActorTemplateIndex = EnemyTypes[(int32)EEnemyType::RatElite];
				break;
			default:
				ActorTemplateIndex = EnemyTypes[(int32)EEnemyType::Rat];
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
	const float BurnDamage = 1.0f;
	const float BurnTime = 1.0f;
	const float PoisonDamage = 0.5f;
	const float PoisonTime = 2.0f;
	const float KnockbackDistance = 1.0f;
	const float KnockbackTime = 1.0f;
	const float SlowdownFactor = 1.0f;
	const float SlowdownTime = 1.0f;
	const float StunTime = 1.0f;
	const float TICK = 0.1f;
	//UE_LOG(LogTemp, Log, TEXT("ENEMY-SYSTEM DAMAGING_ENEMY STATUS CONDITION1: %d"), DamageEffect);
	
	const FMassEntityHandle& Entity = Enemy->Entity;
	if (EntityManager->IsEntityValid(Entity))
	{
		FHealthFragment& HealthFragment = EntityManager->GetFragmentDataChecked<FHealthFragment>(Entity);
		FEnemyStatusCondition& EnemyStatusCondition = EntityManager->GetFragmentDataChecked<FEnemyStatusCondition>(Entity);
		FTransform& Transform = EntityManager->GetFragmentDataChecked<FTransformFragment>(Entity).GetMutableTransform();
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
						EnemyComponent->Die(DamageType, DamageEffect);
					}
					else
					{
						Actor->Destroy();
					}
				}
			}

			EntityManager->DestroyEntity(Entity);
		}
		else
		{
			/* APPLYING DAMAGE EFFECT */
			//UE_LOG(LogTemp, Log, TEXT("ENEMY-SYSTEM DAMAGING_ENEMY STATUS CONDITION2: %d"), DamageEffect);
			switch (DamageEffect)
			{
				case EAttackDamageEffect::BURN:
					EnemyStatusCondition.BurnDamage = BurnDamage;
					if (BurnTime > EnemyStatusCondition.BurnTimer)
					{
						EnemyStatusCondition.BurnTimer = BurnTime;
					}
					break;

				case EAttackDamageEffect::POISON:
					EnemyStatusCondition.PoisonDamage = PoisonDamage * HealthFragment.MaxHealth / (PoisonTime / TICK);
					if (PoisonTime > EnemyStatusCondition.PoisonTimer)
					{
						EnemyStatusCondition.PoisonTimer = PoisonTime;
					}
					break;

				case EAttackDamageEffect::ELECTROCUTION:
					break;

				case EAttackDamageEffect::SLEEP:
					break;

				case EAttackDamageEffect::CONFUSION:
					break;

				case EAttackDamageEffect::KNOCKBACK:
					FVector PlayerEnemyDirection = Transform.GetLocation() - TargetsPositions[PlayerTargetIndex];
					PlayerEnemyDirection.Normalize();
					EnemyStatusCondition.KnockbackDirection = PlayerEnemyDirection;
					EnemyStatusCondition.KnockbackVelocity = 2 * KnockbackDistance / KnockbackTime;
					EnemyStatusCondition.KnockbackDecceleration = -EnemyStatusCondition.KnockbackVelocity / KnockbackTime;
					break;

				case EAttackDamageEffect::STUN:
					if (EnemyStatusCondition.StunTimer <= 0.0f && EnemyStatusCondition.StunCooldownTimer <= 0.0f)
					{
						EnemyStatusCondition.StunTimer = StunTime;
					}
					break;
			}
		}

		return HealthFragment.Health / HealthFragment.MaxHealth;
	}
	return 0.0f;
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