#pragma once

#include "CoreMinimal.h"

struct FSteeringOutput final
{
	FVector2D LinearVelocity{};// Units/sec

	explicit FSteeringOutput(const FVector2D& _UnitsPerSec = {}, float const _DegPerSec = 0.f)
	{
		LinearVelocity = _UnitsPerSec;
	}

	FSteeringOutput& operator=(const FSteeringOutput& other)
	{
		LinearVelocity = other.LinearVelocity;

		return *this;
	}

	FSteeringOutput& operator+(const FSteeringOutput& other)
	{
		LinearVelocity += other.LinearVelocity;

		return *this;
	}

	FSteeringOutput& operator*=(const FSteeringOutput& other)
	{
		LinearVelocity = LinearVelocity * other.LinearVelocity;

		return *this;
	}

	FSteeringOutput& operator*=(float f)
	{
		LinearVelocity = f * LinearVelocity;

		return *this;
	}

	FSteeringOutput& operator/=(float f)
	{
		LinearVelocity = LinearVelocity / f;

		return *this;
	}
};
