#include "Character/HealthComponent.h"

UHealthComponent::UHealthComponent()
	: MaxHealth(100.0f)
	, CurrentHealth(MaxHealth)
	, HealthRegenerationRate(0.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UHealthComponent::BeginPlay()
{
	CurrentHealth = MaxHealth;
	AActor *Owner = GetOwner();

	Owner->OnTakePointDamage.AddDynamic(this, &UHealthComponent::TakeDamage);

	Super::BeginPlay();
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentHealth = FMath::Clamp(CurrentHealth + HealthRegenerationRate * DeltaTime, 0.0f, MaxHealth);
}

void UHealthComponent::TakeDamage(AActor *DamagedActor, float Damage, AController *InstigatedBy, FVector HitLocation, UPrimitiveComponent *FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType *DamageType, AActor *DamageCauser)
{
	if (Damage == 0.0f) 
	{
		return;
	}

	AActor *Causer = nullptr;
	if (InstigatedBy)
	{
		Causer = InstigatedBy->GetPawn();
	}

	if (Damage > 0.0f)
	{
		Injure(FMath::Abs(Damage), Causer);
	}
	else if (CurrentHealth != MaxHealth) 
	{
		Heal(FMath::Abs(Damage), Causer);
	}

	if (CurrentHealth == 0.0f) 
	{
		OnZeroHealth(Causer);
	}
}

void UHealthComponent::OnZeroHealth(AActor *Causer) const
{
	OnDeath.Broadcast(Causer);
}

void UHealthComponent::Heal(float Amount, AActor* Causer)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	OnHeal.Broadcast(Causer);
}

void UHealthComponent::Injure(float Amount, AActor* Causer)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0.0f, MaxHealth);
	OnInjure.Broadcast(Causer);
}

void UHealthComponent::InstantKill(AActor *DamagedActor, UPrimitiveComponent *FHitComponent, FVector ShotFromDirection, const UDamageType *DamageType, AActor *DamageCauser)
{
	if (CurrentHealth <= 0.0f)
	{
		return;
	}

	CurrentHealth = 0.0f;
	OnZeroHealth(DamageCauser);
}

float UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}

float UHealthComponent::GetCurrentHealth()
{
	return CurrentHealth;
}

void UHealthComponent::SetCurrentHealth(float Value)
{
	CurrentHealth = Value;
}