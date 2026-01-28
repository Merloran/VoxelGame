// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyComponent.h"

// Sets default values for this component's properties
UEnemyComponent::UEnemyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    
}

void UEnemyComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!EnemyAttackAudioComponent)
    {
        EnemyAttackAudioComponent = NewObject<UAudioComponent>(this);
        EnemyAttackAudioComponent->RegisterComponent();
        EnemyAttackAudioComponent->AttachToComponent(
            GetOwner()->GetRootComponent(),
            FAttachmentTransformRules::KeepRelativeTransform
        );
    }

    EnemyAttackAudioComponent->SetSound(EnemyAttackSound);
    EnemyAttackAudioComponent->bAutoActivate = false;
    EnemyAttackAudioComponent->AttenuationSettings = SoundAttenuation;
}

void UEnemyComponent::Die(EAttackDamageType DamageType)
{
	OnEnemyDeath.Broadcast(DamageType);
}

void UEnemyComponent::PerformAttack()
{
    if (EnemyAttackAudioComponent && EnemyAttackSound)
    {
        EnemyAttackAudioComponent->Play();
    }
}