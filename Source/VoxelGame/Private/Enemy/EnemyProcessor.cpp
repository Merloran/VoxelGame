// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/EnemyProcessor.h"

#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "MassEntityView.h"
#include "MassExecutionContext.h"
#include "MassEntityQuery.h"
#include "MassProcessingPhaseManager.h"
#include "MassProcessingTypes.h"
#include "MassProcessor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UEnemyProcessor::UEnemyProcessor() : EntityQuery(*this)
{
    //ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::PrePhysics;
    //ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
    //ExecutionFlags = (int32)EProcessorExecutionFlags::All;
    //SetProcessingPhase(EMassProcessingPhase::PrePhysics);
    //ExecutionOrder.ExecuteInGroup = NAME_None;
    //ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
    //ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::Movement);


    //bAutoRegisterWithProcessingPhases = true;
    //ExecutionFlags = (int32)EProcessorExecutionFlags::All;
    //ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::Avoidance);
    //QueryBasedPruning = EMassQueryBasedPruning::Never;


    ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
    SetProcessingPhase(EMassProcessingPhase::PrePhysics);
    ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::AllWorldModes);
    bAutoRegisterWithProcessingPhases = true;
}

void UEnemyProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
    /** QUERY SETTINGS **/
    // Initialize the query (you can pass it in the parent processor constructor as well but this is more explicit)
    //EntityQuery.Initialize(EntityManager);

    /* TAGS */
    // MyQuery.AddTagRequirement<FTemplateRTag>(EMassFragmentPresence::All); // Template

    /* FRAGMENTS */
    EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FEnemyFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FEnemyRepresentationFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FMassRepresentationFragment>(EMassFragmentAccess::ReadWrite);

    /* SHARED FRAGMENTS */
    // MyQuery.AddSharedRequirement<FTemplatedSharedFragment>(EMassFragmentAccess::ReadWrite);

    /* SUBSYSTEMS */
    // MyQuery.AddSubsystemRequirement<UMassDebuggerSubsystem>(EMassFragmentAccess::ReadWrite);

    //EntityQuery.RegisterWithProcessor(*this);// Only needed if the ctor did not initialize the query by passing in itself


    /** PROCESSOR SETTINGS **/

    /* SUBSYSTEMS */ // Subsystems that will be accessed in processor
    // ProcessorRequirements.AddSubsystemRequirement<UMassDebuggerSubsystem>(EMassFragmentAccess::ReadWrite);
}

void UEnemyProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
    EntityQuery.ForEachEntityChunk(Context, [&](FMassExecutionContext& Context)
        {
            const int32 NumEntities = Context.GetNumEntities();
            TArrayView<FTransformFragment> TransformList = Context.GetMutableFragmentView<FTransformFragment>();
            TArrayView<FEnemyFragment> EnemyList = Context.GetMutableFragmentView<FEnemyFragment>();

            for (int32 i = 0; i < NumEntities; i++)
            {
                FTransformFragment& Transform = TransformList[i];
                FEnemyFragment& Enemy = EnemyList[i];

                FVector Current = Transform.GetTransform().GetLocation();
                FVector Target = Enemy.TargetPosition;
                FVector Dir = (Target - Current);
                float Distance = Dir.Length();

                

                // Check distance
                if (Distance < Enemy.AttackRange)
                {
                    // Example: Draw a red sphere to visualize "attack range reached"
                    DrawDebugSphere(Context.GetWorld(), Current, 20.f, 8, FColor::Red, false, 0.1f);
                }
                else if (Distance > KINDA_SMALL_NUMBER)
                {
                    Dir.Normalize();

                    // Move toward target
                    FVector NewPos = Current + Dir * Enemy.MoveSpeed * Context.GetDeltaTimeSeconds();
                    Transform.GetMutableTransform().SetLocation(NewPos);
                    DrawDebugSphere(Context.GetWorld(), Current, 10.f, 8, FColor::Green, false, 0.1f);
                }
            }
        });
}