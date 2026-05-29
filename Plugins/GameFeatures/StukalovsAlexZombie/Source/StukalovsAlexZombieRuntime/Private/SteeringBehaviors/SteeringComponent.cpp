// Fill out your copyright notice in the Description page of Project Settings.

#include "SteeringBehaviors/SteeringComponent.h"

#include "SteeringHelpers.h"
#include "SteeringBehaviors/SteeringBehaviors.h"
#include "Survivor/SurvivorPawn.h"

USteeringComponent::USteeringComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Initializing the steering behaviors
	Behaviors.emplace(typeid(FSeek), std::make_unique<FSeek>());

	// Selecting the starting behavior
	CurrentBehavior = Behaviors.at(typeid(FSeek)).get();
}

void USteeringComponent::BeginPlay()
{
	Super::BeginPlay();
}

FVector2D USteeringComponent::GetOwnerLocation2D() const noexcept
{
	FVector const OwnerLocation{ GetOwner()->GetActorLocation() };
	return {OwnerLocation.X, OwnerLocation.Y};
}

void USteeringComponent::TickComponent(float const DeltaSec, ELevelTick const TickType,
                                       FActorComponentTickFunction* const ThisTickFunction)
{
	Super::TickComponent(DeltaSec, TickType, ThisTickFunction);
	// 1. Calculating the steering
	FSteeringOutput const Steering{ CurrentBehavior->CalculateSteering(DeltaSec, *this) };
	// 2. Applying the movement input to the owner
	ASurvivorPawn* const SurvivorPawn{ Cast<ASurvivorPawn>(GetOwner()) };
	verify(SurvivorPawn);
	SurvivorPawn->AddMovementInput(FVector{Steering.LinearVelocity, 0.f}, 1.f);
	// 3. Setting owner's rotation
	FRotator Rotator{ GetOwner()->GetActorRotation() };
	Rotator.Yaw = FMath::RadiansToDegrees(FMath::Atan2(Steering.LinearVelocity.Y, Steering.LinearVelocity.X));
	GetOwner()->SetActorRotation(Rotator);
}

void USteeringComponent::SetTarget(FVector2D const& Target) const
{
	CurrentBehavior->SetTarget(Target);
}
