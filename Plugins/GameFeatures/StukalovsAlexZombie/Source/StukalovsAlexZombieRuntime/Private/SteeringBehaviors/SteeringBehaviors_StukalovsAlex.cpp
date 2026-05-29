#include "SteeringBehaviors/SteeringBehaviors_StukalovsAlex.h"
#include "SteeringBehaviors/SteeringComponent_StukalovsAlex.h"
#include "DrawDebugHelpers.h"

FSeek_StukalovsAlex::FSeek_StukalovsAlex(float const DegPerSec) noexcept
	: DegPerSec{DegPerSec}
{}

FSeek_StukalovsAlex::~FSeek_StukalovsAlex() = default;

FSteeringOutput_StukalovsAlex FSeek_StukalovsAlex::CalculateSteering(float DeltaTime, USteeringComponent_StukalovsAlex const& SteeringComponent) noexcept
{
	FVector2D LinearVelocity{ Target - SteeringComponent.GetOwnerLocation2D() };
	LinearVelocity.Normalize();
	return FSteeringOutput_StukalovsAlex{LinearVelocity};
}
