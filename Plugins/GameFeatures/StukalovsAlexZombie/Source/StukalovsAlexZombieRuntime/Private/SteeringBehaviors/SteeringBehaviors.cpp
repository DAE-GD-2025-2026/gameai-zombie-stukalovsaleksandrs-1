#include "SteeringBehaviors/SteeringBehaviors.h"
#include "SteeringBehaviors/SteeringComponent.h"
#include "DrawDebugHelpers.h"

FSeek::FSeek(float const DegPerSec) noexcept
	: DegPerSec{DegPerSec}
{}

FSeek::~FSeek() = default;

FSteeringOutput FSeek::CalculateSteering(float DeltaTime, USteeringComponent const& SteeringComponent) noexcept
{
	FVector2D LinearVelocity{ Target - SteeringComponent.GetOwnerLocation2D() };
	LinearVelocity.Normalize();
	return FSteeringOutput{LinearVelocity};
}
