// Fill out your copyright notice in the Description page of Project Settings.


#include "MassEntityExtensions/MassEntityCollisionsSubsystem.h"

int UMassEntityCollisionsSubsystem::CreateBoxCollider(FVector3f Position, FVector3f Size)
{
	FVector3f halfSize = 0.5f * Size;
	_BoxColliders.Emplace(FBoxCollider {
		.id = _NewBoxColliderIndex,
		.active = true,
		.front = Position.X + halfSize.X,
		.back = Position.X - halfSize.X,
		.right = Position.Y + halfSize.Y,
		.left = Position.Y - halfSize.Y,
		.up = Position.Z + halfSize.Z,
		.down = Position.Z - halfSize.Z
	});
	return _NewBoxColliderIndex++;
}

void UMassEntityCollisionsSubsystem::DeleteBoxCollider(int Id)
{
	int first = 0;
	int mid;
	int last = _BoxColliders.Num();

	while (first != last)
	{
		mid = (first + last) / 2;
		if (_BoxColliders[first].id == Id)
		{
			first = last = mid;
		}
		else if (_BoxColliders[first].id < Id)
		{
			first = mid + 1;
		}
		else
		{
			last = mid;
		}
	}

	if (first < _BoxColliders.Num())
	{
		if (_BoxColliders[first].id == Id)
		{
			_BoxColliders.RemoveAt(first);
		}
	}
}

void UMassEntityCollisionsSubsystem::SetColliderActive(int Id, bool Active)
{
	int first = 0;
	int mid;
	int last = _BoxColliders.Num();

	while (first != last)
	{
		mid = (first + last) / 2;
		if (_BoxColliders[first].id == Id)
		{
			first = last = mid;
		}
		else if (_BoxColliders[first].id < Id)
		{
			first = mid + 1;
		}
		else
		{
			last = mid;
		}
	}

	if (first < _BoxColliders.Num())
	{
		if (_BoxColliders[first].id == Id)
		{
			_BoxColliders[first].active = Active;
		}
	}
}

TArrayView<FBoxCollider> UMassEntityCollisionsSubsystem::GetBoxColliders()
{
	return _BoxColliders;
}
