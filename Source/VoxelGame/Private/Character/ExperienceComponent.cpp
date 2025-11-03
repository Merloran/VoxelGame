#include "Character/ExperienceComponent.h"


UExperienceComponent::UExperienceComponent()
	: ownedExperience(0)
	, level(0)
{
	PrimaryComponentTick.bCanEverTick = true;
	SphereRadius = 32.0f;
}

void UExperienceComponent::gain_experience(const int64 experience)
{
	onGainExperience.Broadcast(experience);
	ownedExperience += experience;
	update_level();
}

int64 UExperienceComponent::get_current_experience() const
{
	return ownedExperience;
}

int32 UExperienceComponent::get_level() const
{
	return level;
}

void UExperienceComponent::reset_level()
{
	ownedExperience = 0;
	update_level();
}

float UExperienceComponent::get_experience_progress() const
{
	int64 experienceForCurrentLevel = EXPERIENCE_FOR_FIRST_LEVEL * int64(1) << (level - 1);
	int64 experienceForNextLevel = EXPERIENCE_FOR_FIRST_LEVEL * int64(1) << level;

	if (experienceForNextLevel == experienceForCurrentLevel)
	{
		return 1.0f;
	}

	return float(ownedExperience - experienceForCurrentLevel) / float(experienceForNextLevel - experienceForCurrentLevel);
}

void UExperienceComponent::BeginPlay()
{
	Super::BeginPlay();

	update_level();
	
	OnComponentBeginOverlap.AddDynamic(this, &UExperienceComponent::OnSphereBeginOverlap);
}

void UExperienceComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner())
	{
		OnPickUp.Broadcast(OtherActor);
	}
}

void UExperienceComponent::update_level()
{
	int32 previousLevel = level;
	level = FMath::Log2(double(int64(ownedExperience * 2 / EXPERIENCE_FOR_FIRST_LEVEL)));

	if (level > previousLevel)
	{
		onLevelUp.Broadcast();
	}
}
