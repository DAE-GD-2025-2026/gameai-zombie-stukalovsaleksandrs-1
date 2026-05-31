#pragma once

#include "CoreMinimal.h"

struct FSteeringOutput_StukalovsAlex final
{
	FVector2D LinearVelocity{};// Units/sec

	FSteeringOutput_StukalovsAlex& operator=(const FSteeringOutput_StukalovsAlex& other)
	{
		LinearVelocity = other.LinearVelocity;
		return *this;
	}

};
