// Fill out your copyright notice in the Description page of Project Settings.

#include "Tasks/BTT_EnterHouse_StukalovsAlex.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Engine/World.h"
#include "SteeringBehaviors/SteeringComponent_StukalovsAlex.h"
#include "Tasks/BTTUtils_StukalovsAlex.h"
#include "Village/House/House.h"

#define DEBUG_WAYPOINTS

struct FEnterHouseMemory final
{
	TArray<FVector> Path;
	int32 CurrentPointIdx{};
};

UBTT_EnterHouse_StukalovsAlex::UBTT_EnterHouse_StukalovsAlex()
{
	bNotifyTick = true;// For the TickTask to be called
	NodeName = "EnterHouse";
}

EBTNodeResult::Type UBTT_EnterHouse_StukalovsAlex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 1. Getting the owner
	ASurvivorPawn* SurvivorPawn{ BTTUtils_StukalovsAlex::GetOwner(OwnerComp) };
	if (!SurvivorPawn) return EBTNodeResult::Failed;
	
	// Getting the currently visible house
	UBlackboardComponent* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	verify(BlackboardComponent);
	AHouse* const House{ Cast<AHouse>(BlackboardComponent->GetValueAsObject(TEXT("House"))) };
	verify(House);
	
	// Already inside the house -> Skipping
	if (BTTUtils_StukalovsAlex::IsPointInHouse(SurvivorPawn->GetActorLocation(), House->GetBounds()))
	{
		return EBTNodeResult::Failed;
	}
	
	// Going towards the house. Setting steering behavior to seek
	USteeringComponent_StukalovsAlex* SteeringComponent{ SurvivorPawn->GetComponentByClass<USteeringComponent_StukalovsAlex>() };
	verify(SteeringComponent);
	SteeringComponent->SetBehavior<FSeek_StukalovsAlex>();
	
	// Taking the center of the house
	FVector const HouseCenter{ House->GetBounds().Origin };
	
	// Using the SurvivorPawn::CalculatePath to get the points a character has to travel
	TArray const Path{
		SurvivorPawn->CalculatePath(HouseCenter)
	};
	
	// 6. Writing the path's data to the node's memory block to access it in Tick()
	FEnterHouseMemory* Memory{ reinterpret_cast<FEnterHouseMemory*>(NodeMemory) };
	Memory->Path = Path;
	Memory->CurrentPointIdx = 1;// Skipping SurvivorPawn's own location

	// Visualizing waypoints
#ifdef DEBUG_WAYPOINTS
	for (FVector const Waypoint : Path)
	{
		DrawDebugSphere(GetWorld(), Waypoint, 50, 32, FColor::Green, true);
	}
#endif
	
	// 7. Requesting BT to keep the task alive
	return EBTNodeResult::InProgress;
}

void UBTT_EnterHouse_StukalovsAlex::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// 1. Getting the owner
	ASurvivorPawn* SurvivorPawn{BTTUtils_StukalovsAlex::GetOwner(OwnerComp) };
	if (!SurvivorPawn)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return;
	}
	// 2. Finishing if the entire path was consumed
	FEnterHouseMemory* Memory = reinterpret_cast<FEnterHouseMemory*>(NodeMemory);
	if (Memory->CurrentPointIdx >= Memory->Path.Num())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	// 3. Saving the current waypoint data
	FVector const& CurrentWaypoint{ Memory->Path[Memory->CurrentPointIdx] };

	// 4. Advancing to the next waypoint if the current one is reached
	if ((CurrentWaypoint - SurvivorPawn->GetActorLocation()).SizeSquared() <= WaypointAcceptanceRadius * WaypointAcceptanceRadius)
	{
		++Memory->CurrentPointIdx;
	}
	else
	{
		// 5. Setting the waypoint as arrival destination
		BTTUtils_StukalovsAlex::SetSteeringTarget(*SurvivorPawn, {CurrentWaypoint.X, CurrentWaypoint.Y});
	}
}

EBTNodeResult::Type UBTT_EnterHouse_StukalovsAlex::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 1. Getting the owner
	ASurvivorPawn* const SurvivorPawn = BTTUtils_StukalovsAlex::GetOwner(OwnerComp);
	if (!SurvivorPawn) return EBTNodeResult::Aborted;

	// 2. Removing all the momentum to not distort the other movement input
	SurvivorPawn->GetMovementComponent()->StopMovementImmediately();

	return EBTNodeResult::Aborted;
}

uint16 UBTT_EnterHouse_StukalovsAlex::GetInstanceMemorySize() const
{
	// Without it, the NodeMemory will be of size 0, so any writes will result in segfault
	return sizeof(FEnterHouseMemory);
}
