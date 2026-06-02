#include "SteeringBehaviors/SteeringBehaviors_StukalovsAlex.h"
#include "Components/SteeringComponent_StukalovsAlex.h"
#include "SteeringBehaviors/SteeringOutput_StukalovsAlex.h"
#include "DrawDebugHelpers.h"
#include "Survivor/SurvivorPawn.h"

FVector2D FSteeringBehaviorBase_StukalovsAlex::GetClosestNavigablePoint(FVector2D const Point, AActor& Owner) 
{
	ASurvivorPawn const * const SurvivorPawn{ CastChecked<ASurvivorPawn>(&Owner) };
	auto Path{ SurvivorPawn->CalculatePath(FVector{Point, 0.f}) };
	if (Path.IsEmpty()) return Point;	
	return {Path[1].X, Path[1].Y};
}

FSteeringOutput_StukalovsAlex FIdle_StukalovsAlex::CalculateSteering(float DeltaTime,
                                                                     USteeringComponent_StukalovsAlex const& SteeringComponent) noexcept
{
	return FSteeringOutput_StukalovsAlex{};	
}

FSteeringOutput_StukalovsAlex FSeek_StukalovsAlex::CalculateSteering(float DeltaTime, USteeringComponent_StukalovsAlex const& SteeringComponent) noexcept
{
	// Getting the closest navigable point to the character in the direction of the target
	Target = GetClosestNavigablePoint(Target, *SteeringComponent.GetOwner())t status;
	
	FVector2D LinearVelocity{ Target - SteeringComponent.GetOwnerLocation2D() };
	if (LinearVelocity.SizeSquared() < 1.f)
	{
		LinearVelocity = {};
	}
	else
	{
		LinearVelocity.Normalize();
	}
	return FSteeringOutput_StukalovsAlex{
		LinearVelocity
	};
}

FSteeringOutput_StukalovsAlex FFlight_StukalovsAlex::CalculateSteering(float DeltaTime,
	USteeringComponent_StukalovsAlex const& SteeringComponent) noexcept
{
	// Moving away from the target
	FVector2D const OwnerLocation{ SteeringComponent.GetOwnerLocation2D() };
	FVector2D const OldTarget{ Target };
	FVector2D const LinearVelocity{ OwnerLocation - Target };
	
	Target = OwnerLocation + LinearVelocity * 100.f;

	// Delegating to not duplicate logic
	FSteeringOutput_StukalovsAlex const Output{ FSeek_StukalovsAlex::CalculateSteering(DeltaTime, SteeringComponent) };
	
	Target = OldTarget;

	return Output;
}

FSteeringOutput_StukalovsAlex FLookAt_StukalovsAlex::CalculateSteering(float DeltaTime,
	USteeringComponent_StukalovsAlex const& SteeringComponent) noexcept
{
	FVector2D LinearVelocity{ Target - SteeringComponent.GetOwnerLocation2D() };
	LinearVelocity.Normalize();
	
	return FSteeringOutput_StukalovsAlex{
		LinearVelocity,
	};	
}

FSteeringOutput_StukalovsAlex FWander_StukalovsAlex::CalculateSteering(float DeltaTime,
	USteeringComponent_StukalovsAlex const& SteeringComponent) noexcept
{
	AActor * Agent{ SteeringComponent.GetOwner() };
	verify(Agent);
	
	// Getting random point on the circle
	//// Getting random angle within offset
	float NewTargetDegrees{
		LastTargetDegrees + FMath::FRandRange(-MaxTargetDegreesOffset, MaxTargetDegreesOffset)
	};
	//// Clamping to [0, 360] range
	NewTargetDegrees = FMath::Fmod(FMath::Fmod(NewTargetDegrees, 360.0f) + 360.0f, 360.0f);
    
	//// Getting agent's forward vector. Limiting to 2D since the project is top-down
	FVector2D const AgentForwardVector{ Agent->GetActorForwardVector().X, Agent->GetActorForwardVector().Y };
	//// Saving the angle
	LastTargetDegrees = NewTargetDegrees;
	//// Calculating new target's coordinates
	FVector const AgentLocation{ Agent->GetActorLocation() };
	FVector2D const NewTargetLocation{ FVector2D{AgentLocation.X, AgentLocation.Y} + TargetCircleOffset  * AgentForwardVector + TargetCircleRadius * FVector2D(FMath::Cos(FMath::DegreesToRadians(NewTargetDegrees)), FMath::Sin(FMath::DegreesToRadians(NewTargetDegrees)) ) };
	Target = NewTargetLocation;
	
	// Steering
	return FSeek_StukalovsAlex::CalculateSteering(DeltaTime, SteeringComponent);
}

#pragma region BlendedSteering
FBlendedSteering_StukalovsAlex::FBlendedSteering_StukalovsAlex(std::vector<FWeightedBehavior_StukalovsAlex> && _WeightedBehaviors)
	: WeightedBehaviors{ std::move(_WeightedBehaviors) } {}

FSteeringOutput_StukalovsAlex FBlendedSteering_StukalovsAlex::CalculateSteering(float DeltaTime,
	USteeringComponent_StukalovsAlex const& SteeringComponent) noexcept
{
	FSteeringOutput_StukalovsAlex BlendedSteering = {};
	// We have multiple steering behaviors that we want to switch between right now
	// 1. Iterate over all the weighted steering behaviors
	for (const auto& [Behavior, Weight] : WeightedBehaviors)
	{
		// Not processing behaviors with weights close to 0 
		if (Weight < FLT_EPSILON) continue;
		FSteeringOutput_StukalovsAlex WeightedBehaviorSteering{ Behavior->CalculateSteering(DeltaTime, SteeringComponent) };
		// 2. We have 2 variables, values for which we have to calculate: direction and angular velocity
		// 2.1. Direction
		BlendedSteering.LinearVelocity += WeightedBehaviorSteering.LinearVelocity.GetSafeNormal() * Weight;
	}

	// BlendedSteering.LinearVelocity.Normalize();
	return BlendedSteering;
}
#pragma endregion BlendedSteering
