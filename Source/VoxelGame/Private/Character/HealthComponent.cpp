#include "Character/HealthComponent.h"

UHealthComponent::UHealthComponent()
	: maxHealth(100.0f)
	, currentHealth(maxHealth)
	, healthRegenerationRate(0.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UHealthComponent::BeginPlay()
{
	currentHealth = maxHealth;
	AActor *Owner = GetOwner();

	Owner->OnTakePointDamage.AddDynamic(this, &UHealthComponent::TakeDamage);

	Super::BeginPlay();
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	currentHealth = FMath::Clamp(currentHealth + healthRegenerationRate * DeltaTime, 0.0f, maxHealth);
}

void UHealthComponent::TakeDamage(AActor *DamagedActor, float Damage, AController *InstigatedBy, FVector HitLocation, UPrimitiveComponent *FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType *DamageType, AActor *DamageCauser)
{
	if (Damage == 0.0f) 
	{
		return;
	}

	AActor *causer = nullptr;
	if (InstigatedBy)
	{
		causer = InstigatedBy->GetPawn();
	}

	if (Damage > 0.0f)
	{
		injure(FMath::Abs(Damage), causer);
	}
	else if (currentHealth != maxHealth) 
	{
		heal(FMath::Abs(Damage), causer);
	}

	if (currentHealth == 0.0f) 
	{
		on_zero_health(causer);
	}
}

void UHealthComponent::on_zero_health(AActor *causer) const
{
	onDeath.Broadcast(causer);
}

void UHealthComponent::heal(float amount, AActor* causer)
{
	currentHealth = FMath::Clamp(currentHealth + amount, 0.0f, maxHealth);
	onHeal.Broadcast(causer);
}

void UHealthComponent::injure(float amount, AActor* causer)
{
	currentHealth = FMath::Clamp(currentHealth - amount, 0.0f, maxHealth);
	onInjure.Broadcast(causer);
}

void UHealthComponent::InstantKill(AActor *DamagedActor, UPrimitiveComponent *FHitComponent, FVector ShotFromDirection, const UDamageType *DamageType, AActor *DamageCauser)
{
	if (currentHealth <= 0.0f)
	{
		return;
	}

	currentHealth = 0.0f;
	on_zero_health(DamageCauser);
}

float UHealthComponent::get_max_health()
{
	return maxHealth;
}

float UHealthComponent::get_current_health()
{
	return currentHealth;
}

void UHealthComponent::set_current_health(float value)
{
	currentHealth = value;
}