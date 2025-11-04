#include "Character/ExperienceComponent.h"


UExperienceComponent::UExperienceComponent()
	: OwnedExperience(0)
	, Level(0)
{
	PrimaryComponentTick.bCanEverTick = true;
	SphereRadius = 200.0f;
}

void UExperienceComponent::GainExperience(const int64 Experience)
{
	OnGainExperience.Broadcast(Experience);
	OwnedExperience += Experience;
	UpdateLevel();
}

int64 UExperienceComponent::GetCurrentExperience() const
{
	return OwnedExperience;
}

int32 UExperienceComponent::GetLevel() const
{
	return Level;
}

void UExperienceComponent::ResetLevel()
{
	OwnedExperience = 0;
	UpdateLevel();
}

float UExperienceComponent::GetExperienceProgress() const
{
	int64 experienceForCurrentLevel = EXPERIENCE_FOR_FIRST_LEVEL * int64(1) << (Level - 1);
	int64 experienceForNextLevel = EXPERIENCE_FOR_FIRST_LEVEL * int64(1) << Level;

	if (experienceForNextLevel == experienceForCurrentLevel)
	{
		return 1.0f;
	}

	return float(OwnedExperience - experienceForCurrentLevel) / float(experienceForNextLevel - experienceForCurrentLevel);
}

void UExperienceComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateLevel();

	OnComponentBeginOverlap.AddDynamic(this, &UExperienceComponent::OnSphereBeginOverlap);
}

void UExperienceComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner())
	{
		OnPickUp.Broadcast(OtherActor);
	}
}

void UExperienceComponent::UpdateLevel()
{
	int32 previousLevel = Level;
	Level = FMath::Log2(double(int64(OwnedExperience * 2 / EXPERIENCE_FOR_FIRST_LEVEL)));

	if (Level > previousLevel)
	{
		OnLevelUp.Broadcast();
	}
}
