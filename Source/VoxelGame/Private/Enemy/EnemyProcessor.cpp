// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/EnemyProcessor.h"
#include "Enemy/EnemyFragment.h"
#include "Enemy/EnemySubsystem.h"

#include "MassCommonFragments.h"
#include "MassRepresentationFragments.h"
#include "MassActorSubsystem.h"
#include "MassRepresentationSubsystem.h"
#include "MassExecutionContext.h"
#include "DrawDebugHelpers.h"

UEnemyProcessor::UEnemyProcessor() : EntityQuery(*this)
{
    ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
    SetProcessingPhase(EMassProcessingPhase::PrePhysics);
    ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::AllWorldModes);
    bAutoRegisterWithProcessingPhases = true;
}

void UEnemyProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
    /* FRAGMENTS */
    EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FEnemyFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FMassRepresentationFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FMassRepresentationLODFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FMassActorFragment>(EMassFragmentAccess::ReadWrite);

    /* SUBSYSTEMS */
    EntityQuery.AddSubsystemRequirement<UMassRepresentationSubsystem>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddSubsystemRequirement<UEnemySubsystem>(EMassFragmentAccess::ReadWrite);
}

void UEnemyProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
    EntityQuery.ForEachEntityChunk(Context, [this](FMassExecutionContext& Context)
        {
            const int32 NumEntities = Context.GetNumEntities();
            const TArrayView<FTransformFragment> TransformList = Context.GetMutableFragmentView<FTransformFragment>();
            const TArrayView<FEnemyFragment> EnemyList = Context.GetMutableFragmentView<FEnemyFragment>();
            //const TArrayView<FMassRepresentationFragment> RepresentationList = Context.GetMutableFragmentView<FMassRepresentationFragment>();
            //const TConstArrayView<FMassRepresentationLODFragment> LODList = Context.GetFragmentView<FMassRepresentationLODFragment>();
            const TArrayView<FMassActorFragment> ActorList = Context.GetMutableFragmentView<FMassActorFragment>();

            UMassRepresentationSubsystem& RepSubsystem = Context.GetMutableSubsystemChecked<UMassRepresentationSubsystem>();
            UEnemySubsystem& EnemySubsystem = Context.GetMutableSubsystemChecked<UEnemySubsystem>();

            const float DeltaTime = Context.GetDeltaTimeSeconds();

            const float radius = 50.f;
            const float radius2 = 50.f;

            for (int32 i = 0; i < NumEntities; ++i)
            {
                FTransform& Transform = TransformList[i].GetMutableTransform();
                FEnemyFragment& Enemy = EnemyList[i];
                //const FMassRepresentationLODFragment& LOD = LODList[i];
                //const FMassRepresentationFragment& Representation = RepresentationList[i];
                FMassActorFragment& ActorFrag = ActorList[i];

                /* MOVEMENT */
                FVector Current = Transform.GetLocation();
                FVector Direction = EnemySubsystem.TargetPosition - Current;
                Direction.Z = 0.0f;
                //FVector Direction = Enemy.TargetPosition - Current;
                const float Distance = Direction.Size();
                FQuat NewRot = Direction.ToOrientationQuat();
                Transform.SetRotation(NewRot);
                FVector NewPos = Current;

                if (Distance > Enemy.AttackRange)
                {
                    Direction.Normalize();
                    NewPos += Direction * Enemy.MoveSpeed * DeltaTime;
                    Transform.SetLocation(NewPos);
                }


                for (int32 j = 0; j < NumEntities; ++j)
                {
                    FEnemyFragment& OtherEnemy = EnemyList[j];
                    if (Enemy.Id != OtherEnemy.Id)
                    {
                        FTransform& OtherTransform = TransformList[j].GetMutableTransform();
                        FVector OtherPosition = OtherTransform.GetLocation();
                        FVector Difference = NewPos - OtherPosition;
                        const float DifferenceDistance = Difference.Size();
                        Difference.Normalize();
                        if (DifferenceDistance <= radius2)
                        {
                            FVector Delta = 0.55f * (radius2 - DifferenceDistance) * Difference;
                            NewPos += Delta;
                            OtherTransform.SetLocation(OtherPosition - Delta);
                        }
                    }
                }

                if (AActor* Actor = ActorFrag.GetMutable())
                {
                    Actor->SetActorLocationAndRotation(NewPos, NewRot);
                }

                /* ATTACKING */
                if (Enemy.AttackTimer > 0.0f)
                {
                    Enemy.AttackTimer -= DeltaTime;
                }
                else if (Distance <= Enemy.AttackRange)
                {
                    EnemySubsystem.DamageTarget(0, Enemy.Damage);
                    Enemy.AttackTimer = Enemy.AttackCooldown;
                }

                // Debug
                //DrawDebugSphere(Context.GetWorld(), Transform.GetLocation(), 20.f, 8, (Dist < Enemy.AttackRange) ? FColor::Red : FColor::Green, false, 0.1f);
            }
        });
}