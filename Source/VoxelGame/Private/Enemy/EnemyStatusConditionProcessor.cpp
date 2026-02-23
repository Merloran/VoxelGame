// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyStatusConditionProcessor.h"
#include "Enemy/EnemyFragment.h"
#include "Enemy/EnemySubsystem.h"
#include "Enemy/EnemyComponent.h"

#include "MassCommonFragments.h"
#include "MassRepresentationFragments.h"
#include "MassActorSubsystem.h"
#include "MassRepresentationSubsystem.h"
#include "MassExecutionContext.h"


UEnemyStatusConditionProcessor::UEnemyStatusConditionProcessor() : EntityQuery(*this)
{
    ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
    SetProcessingPhase(EMassProcessingPhase::PrePhysics);
    ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::AllWorldModes);
    bAutoRegisterWithProcessingPhases = true;
}

void UEnemyStatusConditionProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
    /* FRAGMENTS */
    EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FEnemyFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FEnemyStatusCondition>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FHealthFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FMassActorFragment>(EMassFragmentAccess::ReadWrite);

    /* SUBSYSTEMS */
    //EntityQuery.AddSubsystemRequirement<UMassRepresentationSubsystem>(EMassFragmentAccess::ReadWrite);
    //EntityQuery.AddSubsystemRequirement<UEnemySubsystem>(EMassFragmentAccess::ReadWrite);
    //EntityQuery.AddSubsystemRequirement<UMassEntityCollisionsSubsystem>(EMassFragmentAccess::ReadWrite);
}

void UEnemyStatusConditionProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
    EntityQuery.ForEachEntityChunk(Context, [this](FMassExecutionContext& Context)
        {
            const int32 NumEntities = Context.GetNumEntities();
            const TArrayView<FEnemyFragment> EnemyList = Context.GetMutableFragmentView<FEnemyFragment>();
            const TArrayView<FEnemyStatusCondition> EnemyStatusConditionList = Context.GetMutableFragmentView<FEnemyStatusCondition>();
            const TArrayView<FHealthFragment> HealthList = Context.GetMutableFragmentView<FHealthFragment>();
            const TArrayView<FMassActorFragment> ActorList = Context.GetMutableFragmentView<FMassActorFragment>();
            const TArrayView<FTransformFragment> TransformList = Context.GetMutableFragmentView<FTransformFragment>();
            
            const float DeltaTime = Context.GetDeltaTimeSeconds();
            const float TICK = 0.1f;

            for (int32 i = 0; i < NumEntities; ++i)
            {
                FTransform& Transform = TransformList[i].GetMutableTransform();
                FEnemyFragment& Enemy = EnemyList[i];
                FHealthFragment& Health = HealthList[i];
                FEnemyStatusCondition& EnemyStatusCondition = EnemyStatusConditionList[i];
                FMassActorFragment& ActorFrag = ActorList[i];
                AActor* Actor = ActorFrag.GetMutable();
                float Damage = 0.0f;

                /* BURN */
                if (EnemyStatusCondition.BurnTimer > 0) 
                {
                    EnemyStatusCondition.BurnTickTimer += DeltaTime;
                    if (EnemyStatusCondition.BurnTickTimer >= TICK)
                    {
                        EnemyStatusCondition.BurnTickTimer -= 0.1f;
                        EnemyStatusCondition.BurnTimer -= 0.1f;
                        Damage += EnemyStatusCondition.BurnDamage;
                    }
                }

                /* POISON */
                if (EnemyStatusCondition.PoisonTimer > 0) 
                {
                    EnemyStatusCondition.PoisonTickTimer += DeltaTime;
                    if (EnemyStatusCondition.PoisonTickTimer >= TICK)
                    {
                        EnemyStatusCondition.PoisonTickTimer -= 0.1f;
                        EnemyStatusCondition.PoisonTimer -= 0.1f;
                        Damage += EnemyStatusCondition.PoisonDamage;
                    }
                }

                if (Damage > 0.0f && Actor) 
                {
				    UEnemyComponent* EnemyComponent = Actor->FindComponentByClass<UEnemyComponent>();
                    Health.Health -= Damage;

				    if (EnemyComponent)
				    {
                        if (Health.Health <= 0.f)
                        {
				            EnemyComponent->Die(EAttackDamageType::None);
                        }
                        else
                        {
                            EnemyComponent->CallOnTakenDamage(Health.Health / Health.MaxHealth);
                        }
                    }
                }
            }
        });
}