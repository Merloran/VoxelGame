// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponSubsystem.h"

#include "Math/UnrealMathUtility.h"
#include "Algo/MinElement.h"
#include "Algo/MaxElement.h"

void UWeaponSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

UTexture2D* UWeaponSubsystem::Initialize(UTexture2D* BurnMap, int32 MapWidth, int32 MapLength, int32 MapHeight, FVector MapOffset)
{
	Width = MapWidth;
	Length = MapLength;
	Height = MapHeight;
	Offset = MapOffset;

	BurnsMap = BurnMap;

	FTexture2DMipMap& Mip = BurnsMap->GetPlatformData()->Mips[0];
	UE_LOG(LogTemp, Warning, TEXT("Texture Size: %d x %d"),
		BurnsMap->GetSizeX(),
		BurnsMap->GetSizeY());
	check(BurnsMap);
	check(BurnsMap->GetPlatformData());
	check(BurnsMap->GetPlatformData()->Mips.Num() > 0);
	uint8* PixelData = reinterpret_cast<uint8*>(Mip.BulkData.Lock(LOCK_READ_WRITE));
	const size_t PixelCount = 4 * Width * Length * Height;
	for (size_t Index = 0; Index < PixelCount; ++Index)
	{
		PixelData[Index] = 0;
	}
	Mip.BulkData.Unlock();
	BurnsMap->UpdateResource();

	return BurnsMap;
}



void UWeaponSubsystem::ProcessCone(FVector Direction, float Radius, FVector BeginPosition, float Angle, float Value)
{

	FVector EndPosition = BeginPosition + Radius * Direction;

	FQuat Quat = Direction.ToOrientationQuat();
	FVector Right = Quat * FVector(0.0f, 1.0f, 0.0f);
	FVector Up = Quat * FVector(0.0f, 0.0f, 1.0f);

	const float Cosine = FMath::Cos(0.5f * Angle);
	const float Hypotenuse = Radius * Cosine;
	FVector CathetusDirection = Radius * FMath::Sin(0.5f * Angle) * Direction;

	FVector RightPosition = BeginPosition + CathetusDirection + Hypotenuse * Right;
	FVector LeftPosition = BeginPosition + CathetusDirection - Hypotenuse * Right;
	FVector UpPosition = BeginPosition + CathetusDirection + Hypotenuse * Up;
	FVector DownPosition = BeginPosition + CathetusDirection - Hypotenuse * Up;

	float Xs[] = { BeginPosition.X, EndPosition.X, RightPosition.X, LeftPosition.X, UpPosition.X, DownPosition.X };
	float Ys[] = { BeginPosition.Y, EndPosition.Y, RightPosition.Y, LeftPosition.Y, UpPosition.Y, DownPosition.Y };
	float Zs[] = { BeginPosition.Z, EndPosition.Z, RightPosition.Z, LeftPosition.Z, UpPosition.Z, DownPosition.Z };

	const float MinX = *Algo::MinElement(Xs);
	const float MaxX = *Algo::MaxElement(Xs);
	const float MinY = *Algo::MinElement(Ys);
	const float MaxY = *Algo::MaxElement(Ys);
	const float MinZ = *Algo::MinElement(Zs);
	const float MaxZ = *Algo::MaxElement(Zs);

	const int32 IndexMinX = FMath::Clamp((int)(MinX / 50.0f), 0, Width - 1);
	const int32 IndexMaxX = FMath::Clamp((int)(MaxX / 50.0f), 0, Width - 1);
	const int32 IndexMinY = FMath::Clamp((int)(MinY / 50.0f), 0, Length - 1);
	const int32 IndexMaxY = FMath::Clamp((int)(MaxY / 50.0f), 0, Length - 1);
	const int32 IndexMinZ = FMath::Clamp((int)(MinZ / 50.0f), 0, Height - 1);
	const int32 IndexMaxZ = FMath::Clamp((int)(MaxZ / 50.0f), 0, Height - 1);

	const uint16 Change = 255 * Value;

	FTexture2DMipMap& Mip = BurnsMap->GetPlatformData()->Mips[0];
	uint8* PixelData = reinterpret_cast<uint8*>(Mip.BulkData.Lock(LOCK_READ_WRITE));

	for (int32 X = IndexMinX; X <= IndexMaxX; ++X)
	{
		for (int32 Y = IndexMinY; Y <= IndexMaxY; ++Y)
		{
			for (int32 Z = IndexMinZ; Z <= IndexMaxZ; ++Z)
			{
				//FVector VoxelCenter(X * 50 + 25, Y * 50 + 25, Z * 50 + 25);
				FVector VoxelCenter(X * 50 + 25 + Offset.X, Y * 50 + 25 + Offset.Y, Z * 50 + 25 + Offset.Z);
				FVector Delta = VoxelCenter - BeginPosition;
				if (Delta.Length() < Radius)
				{
					Delta.Normalize();
					float Dot = Direction.Dot(Delta);
					if (Dot > Cosine)
					{
						//size_t Index = 4 * ((Y + Z * Length) * Width + X);
						size_t Index = ((Y + Z * Length) * Width + X);
						PixelData[Index] = (uint8)FMath::Min(PixelData[Index] + Change, 255); // B
						//PixelData[Index + 1] = PixelData[Index]; // G
						//PixelData[Index + 2] = PixelData[Index]; // R
						//PixelData[Index + 3] = PixelData[Index]; // A
					}
				}
			}
		}
	}

	// Unlock & update
	Mip.BulkData.Unlock();
}

void UWeaponSubsystem::ProcessArea(float Radius, FVector ReferencePosition, float Value)
{
	const float MinX = ReferencePosition.X - Radius;
	const float MaxX = ReferencePosition.X + Radius;
	const float MinY = ReferencePosition.Y - Radius;
	const float MaxY = ReferencePosition.Y + Radius;
	const float MinZ = ReferencePosition.Z - Radius;
	const float MaxZ = ReferencePosition.Z + Radius;

	const int32 IndexMinX = FMath::Clamp((int)(MinX / 50.0f), 0, Width - 1);
	const int32 IndexMaxX = FMath::Clamp((int)(MaxX / 50.0f), 0, Width - 1);
	const int32 IndexMinY = FMath::Clamp((int)(MinY / 50.0f), 0, Length - 1);
	const int32 IndexMaxY = FMath::Clamp((int)(MaxY / 50.0f), 0, Length - 1);
	const int32 IndexMinZ = FMath::Clamp((int)(MinZ / 50.0f), 0, Height - 1);
	const int32 IndexMaxZ = FMath::Clamp((int)(MaxZ / 50.0f), 0, Height - 1);

	const uint16 Change = 255 * Value;

	UE_LOG(LogTemp, Warning, TEXT("Texture Size: %d x %d"),
		BurnsMap->GetSizeX(),
		BurnsMap->GetSizeY());
	check(BurnsMap);
	check(BurnsMap->GetPlatformData());
	check(BurnsMap->GetPlatformData()->Mips.Num() > 0);
	FTexture2DMipMap& Mip = BurnsMap->GetPlatformData()->Mips[0];
	UE_LOG(LogTemp, Warning, TEXT("After Texture Size"))
	
	uint8* PixelData = reinterpret_cast<uint8*>(Mip.BulkData.Lock(LOCK_READ_WRITE));

	for (int32 X = IndexMinX; X <= IndexMaxX; ++X)
	{
		for (int32 Y = IndexMinY; Y <= IndexMaxY; ++Y)
		{
			for (int32 Z = IndexMinZ; Z <= IndexMaxZ; ++Z)
			{
				//FVector VoxelCenter(X * 50 + 25, Y * 50 + 25, Z * 50 + 25);
				FVector VoxelCenter(X * 50 + 25 + Offset.X, Y * 50 + 25 + Offset.Y, Z * 50 + 25 + Offset.Z);
				FVector Delta = VoxelCenter - ReferencePosition;
				if (Delta.Length() < Radius)
				{
					size_t Index = ((Y + Z * Length) * Width + X);
					PixelData[Index] = (uint8)FMath::Min(PixelData[Index] + Change, 255); // B
				}
			}
		}
	}

	// Unlock & update
	Mip.BulkData.Unlock();
}

void UWeaponSubsystem::UpdateTexture(bool bFreeData)
{
	BurnsMap->UpdateResource();
}