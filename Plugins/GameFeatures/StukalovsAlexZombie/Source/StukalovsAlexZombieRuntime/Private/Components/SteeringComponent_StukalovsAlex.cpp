// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SteeringComponent_StukalovsAlex.h"

#include "SteeringBehaviors/SteeringOutput_StukalovsAlex.h"
#include "SteeringBehaviors/SteeringBehaviors_StukalovsAlex.h"
#include "Survivor/SurvivorPawn.h"

USteeringComponent_StukalovsAlex::USteeringComponent_StukalovsAlex()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Initializing the steering behaviors
	Behaviors.emplace(typeid(FIdle_StukalovsAlex), std::make_unique<FIdle_StukalovsAlex>());
	Behaviors.emplace(typeid(FSeek_StukalovsAlex), std::make_unique<FSeek_StukalovsAlex>());
	Behaviors.emplace(typeid(FFlight_StukalovsAlex), std::make_unique<FFlight_StukalovsAlex>());
	Behaviors.emplace(typeid(FLookAt_StukalovsAlex), std::make_unique<FLookAt_StukalovsAlex>());
	Behaviors.emplace(typeid(FWander_StukalovsAlex), std::make_unique<FWander_StukalovsAlex>());

	// Selecting the starting behavior
	CurrentBehavior = Behaviors.at(typeid(FIdle_StukalovsAlex)).get();
}

void USteeringComponent_StukalovsAlex::BeginPlay()
{
	Super::BeginPlay();
	SurvivorPawn = Cast<ASurvivorPawn>(GetOwner());
	verify(SurvivorPawn);
}


FVector2D USteeringComponent_StukalovsAlex::GetOwnerLocation2D() const noexcept
{
	FVector const OwnerLocation{ GetOwner()->GetActorLocation() };
	return {OwnerLocation.X, OwnerLocation.Y};
}

void USteeringComponent_StukalovsAlex::FaceTargetImmidiately()
{
	FVector2D Target{ CurrentBehavior->GetTarget() };
    
	// Get current positions
	FVector const CurrentLocation = SurvivorPawn->GetActorLocation();
	FVector const TargetLocation(Target.X, Target.Y, CurrentLocation.Z);
    
	// Calculate rotation to look at target
	FRotator const NewRotation{ (TargetLocation - CurrentLocation).Rotation() };
    
	SurvivorPawn->SetActorRotation(NewRotation);
}

void USteeringComponent_StukalovsAlex::TickComponent(float const DeltaSec, ELevelTick const TickType,
                                                     FActorComponentTickFunction* const ThisTickFunction)
{
	Super::TickComponent(DeltaSec, TickType, ThisTickFunction);
	if (typeid(CurrentBehavior) == typeid(FIdle_StukalovsAlex)) return;// Not moving in idle state
	// 1. Calculating the steering
	FSteeringOutput_StukalovsAlex const Steering{ CurrentBehavior->CalculateSteering(DeltaSec, *this) };
	// 2. Applying the movement input to the owner
	SurvivorPawn->AddMovementInput(FVector{Steering.LinearVelocity, 0.f}, 1.f);
	// 3. Rotating the character in movement direction
	static constexpr float SharpnessFactor{ 5.f };// How fast the lerp catches up with target (higher - faster, lower - slower)
	SmoothedVelocity = FMath::Lerp(SmoothedVelocity, Steering.LinearVelocity, DeltaSec * SharpnessFactor);
	if (!SmoothedVelocity.IsNearlyZero(0.1f))
	{
		FVector const MoveDir{ SmoothedVelocity.X, SmoothedVelocity.Y, 0.f };
		FRotator const TargetRotation{ MoveDir.Rotation() };
		FRotator const CurrentRotation{ SurvivorPawn->GetActorRotation() };
		FRotator const NewRotation{ FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSec, DegPerSec) };
		SurvivorPawn->SetActorRotation(NewRotation);
	}
}

void USteeringComponent_StukalovsAlex::SetTarget(FVector2D const& Target) const
{
	CurrentBehavior->SetTarget(Target);
}
