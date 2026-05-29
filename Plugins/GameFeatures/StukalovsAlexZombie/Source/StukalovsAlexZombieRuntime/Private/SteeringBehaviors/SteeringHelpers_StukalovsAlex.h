#pragma once

#include "CoreMinimal.h"

struct FSteeringOutput_StukalovsAlex final
{
	FVector2D LinearVelocity{};// Units/sec

	explicit FSteeringOutput_StukalovsAlex(const FVector2D& _UnitsPerSec = {}, float const _DegPerSec = 0.f)
	{
		LinearVelocity = _UnitsPerSec;
	}

	FSteeringOutput_StukalovsAlex& operator=(const FSteeringOutput_StukalovsAlex& other)
	{
		LinearVelocity = other.LinearVelocity;

		return *this;
	}

	FSteeringOutput_StukalovsAlex& operator+(const FSteeringOutput_StukalovsAlex& other)
	{
		LinearVelocity += other.LinearVelocity;

		return *this;
	}

	FSteeringOutput_StukalovsAlex& operator*=(const FSteeringOutput_StukalovsAlex& other)
	{
		LinearVelocity = LinearVelocity * other.LinearVelocity;

		return *this;
	}

	FSteeringOutput_StukalovsAlex& operator*=(float f)
	{
		LinearVelocity = f * LinearVelocity;

		return *this;
	}

	FSteeringOutput_StukalovsAlex& operator/=(float f)
	{
		LinearVelocity = LinearVelocity / f;

		return *this;
	}
};
