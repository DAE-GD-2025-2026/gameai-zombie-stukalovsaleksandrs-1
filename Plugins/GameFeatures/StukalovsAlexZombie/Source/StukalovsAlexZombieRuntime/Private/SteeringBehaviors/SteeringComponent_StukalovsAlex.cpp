// Fill out your copyright notice in the Description page of Project Settings.

#include "SteeringBehaviors/SteeringComponent_StukalovsAlex.h"

#include "SteeringOutput_StukalovsAlex.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SteeringBehaviors/SteeringBehaviors_StukalovsAlex.h"
#include "Survivor/SurvivorPawn.h"

USteeringComponent_StukalovsAlex::USteeringComponent_StukalovsAlex()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Initializing the steering behaviors
	Behaviors.emplace(typeid(FSeek_StukalovsAlex), std::make_unique<FSeek_StukalovsAlex>());
	Behaviors.emplace(typeid(FFlight_StukalovsAlex), std::make_unique<FFlight_StukalovsAlex>());
	Behaviors.emplace(typeid(FLookAt_StukalovsAlex), std::make_unique<FLookAt_StukalovsAlex>());
	Behaviors.emplace(typeid(FWander_StukalovsAlex), std::make_unique<FWander_StukalovsAlex>());

	// Selecting the starting behavior
	CurrentBehavior = Behaviors.at(typeid(FSeek_StukalovsAlex)).get();
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

void USteeringComponent_StukalovsAlex::TickComponent(float const DeltaSec, ELevelTick const TickType,
                                       FActorComponentTickFunction* const ThisTickFunction)
{
	Super::TickComponent(DeltaSec, TickType, ThisTickFunction);
	// 1. Calculating the steering
	FSteeringOutput_StukalovsAlex const Steering{ CurrentBehavior->CalculateSteering(DeltaSec, *this) };
	// 2. Applying the movement input to the owner
	SurvivorPawn->AddMovementInput(FVector{Steering.LinearVelocity, 0.f}, 1.f);
	// 3. Rotating the character in movement direction
	if (!Steering.LinearVelocity.IsNearlyZero())
	{
		FVector const MoveDir{ Steering.LinearVelocity.X, Steering.LinearVelocity.Y, 0.f };
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
