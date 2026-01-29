// Fill out your copyright notice in the Description page of Project Settings.

/* WEAPON */
#include "Weapon/WeaponProcessor.h"
#include "Weapon/WeaponSubsystem.h"

/* ENEMY */
#include "Enemy/EnemyFragment.h"
#include "Enemy/EnemySubsystem.h"

/* MASS ENTITY */
#include "MassCommonFragments.h"
#include "MassActorSubsystem.h"
#include "MassExecutionContext.h"

UWeaponProcessor::UWeaponProcessor() : EntityQuery(*this)
{
    ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
    SetProcessingPhase(EMassProcessingPhase::PrePhysics);
    ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::AllWorldModes);
    bAutoRegisterWithProcessingPhases = true;
}

void UWeaponProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
    /* FRAGMENTS */
    EntityQuery.AddRequirement<FHealthFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FEnemyFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FMassActorFragment>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);

    /* SUBSYSTEMS */
    EntityQuery.AddSubsystemRequirement<UEnemySubsystem>(EMassFragmentAccess::ReadWrite);
    EntityQuery.AddSubsystemRequirement<UWeaponSubsystem>(EMassFragmentAccess::ReadWrite);
}

void UWeaponProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
    EntityQuery.ForEachEntityChunk(Context, [this](FMassExecutionContext& Context)
        {
            const int32 NumEntities = Context.GetNumEntities();
            const TArrayView<FTransformFragment> TransformList = Context.GetMutableFragmentView<FTransformFragment>();
            const TArrayView<FEnemyFragment> EnemyList = Context.GetMutableFragmentView<FEnemyFragment>();
            const TArrayView<FMassActorFragment> ActorList = Context.GetMutableFragmentView<FMassActorFragment>();

            UEnemySubsystem& EnemySubsystem = Context.GetMutableSubsystemChecked<UEnemySubsystem>();
            UWeaponSubsystem& WeaponSubsystem = Context.GetMutableSubsystemChecked<UWeaponSubsystem>();

            const float DeltaTime = Context.GetDeltaTimeSeconds();

            for (int32 i = 0; i < NumEntities; ++i)
            {

            }
        });
}