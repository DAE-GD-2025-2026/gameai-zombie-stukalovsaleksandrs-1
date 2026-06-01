#include "SteeringBehaviors/SteeringBehaviors_StukalovsAlex.h"
#include "Components/SteeringComponent_StukalovsAlex.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "SteeringOutput_StukalovsAlex.h"
#include "Survivor/SurvivorPawn.h"

FSteeringOutput_StukalovsAlex FIdle_StukalovsAlex::CalculateSteering(float DeltaTime,
	USteeringComponent_StukalovsAlex const& SteeringComponent) noexcept
{
	return FSteeringOutput_StukalovsAlex{};	
}

FSteeringOutput_StukalovsAlex FSeek_StukalovsAlex::CalculateSteering(float DeltaTime, USteeringComponent_StukalovsAlex const& SteeringComponent) noexcept
{
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
	FVector2D LinearVelocity{ SteeringComponent.GetOwnerLocation2D() - Target };
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
	FVector2D const NewTargetLocation{ FVector2D{AgentLocation.X, AgentLocation.Y} + TargetCircleOffset  * AgentForwardVector + TargetCircleRadius * FVector2D(FMath::Cos(NewTargetDegrees), FMath::Sin(NewTargetDegrees) ) };

	// Getting the closest navigable point to the character in the direction of the target
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Agent->GetWorld());
	ASurvivorPawn* SurvivorPawn{ Cast<ASurvivorPawn>(Agent) };
	verify(NavSys && SurvivorPawn);
	auto Path{ SurvivorPawn->CalculatePath(FVector{NewTargetLocation, 0.f}) };
	if (not Path.IsEmpty())
	{
		// Note Path[0] is the current location
		SteeringComponent.SetTarget({Path[1].X, Path[1].Y});
	}
	else
	{
		SteeringComponent.SetTarget(NewTargetLocation);
	}

	// Steering
	return FSeek_StukalovsAlex::CalculateSteering(DeltaTime, SteeringComponent);
}
