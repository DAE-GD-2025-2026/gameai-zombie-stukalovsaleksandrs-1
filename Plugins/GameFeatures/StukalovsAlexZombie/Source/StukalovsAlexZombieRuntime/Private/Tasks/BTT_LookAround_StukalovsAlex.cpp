// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_LookAround_StukalovsAlex.h"

#include "BTTUtils_StukalovsAlex.h"
#include "SteeringBehaviors/SteeringComponent_StukalovsAlex.h"
#include "Survivor/SurvivorPawn.h"

UBTT_LookAround_StukalovsAlex::UBTT_LookAround_StukalovsAlex()
{
	NodeName = "LookAround";
}

EBTNodeResult::Type UBTT_LookAround_StukalovsAlex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASurvivorPawn* const SurvivorPawn = BTTUtils_StukalovsAlex::GetOwner(OwnerComp);
	verify(SurvivorPawn);

	USteeringComponent_StukalovsAlex* const SteeringComponent{ SurvivorPawn->GetComponentByClass<USteeringComponent_StukalovsAlex>() };
	verify(SteeringComponent);

	SteeringComponent->SetBehavior<FLookAt_StukalovsAlex>();
	
	// Setting a random LookAt target
	FRotator const CurrentDeg{ SurvivorPawn->GetActorRotation() };
	double const DegToTurn{ FMath::RandRange(-AbsDegToTurn, AbsDegToTurn) },
		TotalRad{ FMath::DegreesToRadians(CurrentDeg.Yaw + DegToTurn) };

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Total degrees = %f"), FMath::RadiansToDegrees(TotalRad)));

	FVector const ActorLocation{ SurvivorPawn->GetActorLocation() };
	FVector2D const Target{ ActorLocation.X + FMath::Cos(TotalRad), ActorLocation.Y + FMath::Sin(TotalRad) };
	SteeringComponent->SetTarget(Target);
	
	return EBTNodeResult::Succeeded;
}

