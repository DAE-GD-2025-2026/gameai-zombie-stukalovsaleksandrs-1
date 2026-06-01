// Fill out your copyright notice in the Description page of Project Settings.

#include "Tasks/BTT_EnterHouse_StukalovsAlex.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HouseTrackerComponent_StukalovsAlex.h"
#include "Survivor/SurvivorPawn.h"
#include "Components/SteeringComponent_StukalovsAlex.h"
#include "Tasks/BTTUtils_StukalovsAlex.h"
#include "Village/House/House.h"

#define DEBUG_WAYPOINTS

struct FEnterHouseMemory final
{
	TArray<FVector> Path;
	uint32_t CurrentPointIdx{};
};

UBTT_EnterHouse_StukalovsAlex::UBTT_EnterHouse_StukalovsAlex()
{
	bNotifyTick = true;// For the TickTask to be called
	NodeName = "EnterHouse";
}

EBTNodeResult::Type UBTT_EnterHouse_StukalovsAlex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Getting the owner
	SurvivorPawn = BTTUtils_StukalovsAlex::GetOwner(OwnerComp);
	if (!SurvivorPawn) return EBTNodeResult::Failed;
	
	// Getting the currently visible house
	UBlackboardComponent* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	verify(BlackboardComponent);
	House = Cast<AHouse>(BlackboardComponent->GetValueAsObject(HouseKey.SelectedKeyName));
	verify(House);
	
	// Already inside the house -> Skipping
	if (BTTUtils_StukalovsAlex::IsPointInHouse(SurvivorPawn->GetActorLocation(), House->GetBounds()))
	{
		return EBTNodeResult::Failed;
	}
	

	// Getting the HouseTrackerComponent
	HouseTrackerComponent = SurvivorPawn->GetComponentByClass<UHouseTrackerComponent_StukalovsAlex>();
	verify(HouseTrackerComponent);

	// House already visited -> skipping
	if (HouseTrackerComponent->IsHouseVisited(*House)) return EBTNodeResult::Failed;
	
	// Requesting to look around the house
	UBlackboardComponent& Blackboard{ BTTUtils_StukalovsAlex::GetBlackboard(OwnerComp) };
	Blackboard.SetValueAsBool(ShouldLookAroundKey.SelectedKeyName, true);
	
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
	
	// Writing the path's data to the node's memory block to access it in Tick()
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
	
	// Getting the owner
	SurvivorPawn = BTTUtils_StukalovsAlex::GetOwner(OwnerComp);
	verify(SurvivorPawn);

	
	
	// 7. Requesting BT to keep the task alive
	return EBTNodeResult::InProgress;
}

void UBTT_EnterHouse_StukalovsAlex::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Finishing if the entire path was consumed
	FEnterHouseMemory* Memory = reinterpret_cast<FEnterHouseMemory*>(NodeMemory);
	if (Memory->CurrentPointIdx >= static_cast<uint32_t>(Memory->Path.Num()))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	// Saving the current waypoint data
	FVector const& CurrentWaypoint{ Memory->Path[Memory->CurrentPointIdx] };

	// Advancing to the next waypoint if the current one is reached
	if ((CurrentWaypoint - SurvivorPawn->GetActorLocation()).SizeSquared() <= WaypointAcceptanceRadius * WaypointAcceptanceRadius)
	{
		++Memory->CurrentPointIdx;
	}
	else
	{
		// Setting the waypoint as arrival destination
		BTTUtils_StukalovsAlex::SetSteeringTarget(*SurvivorPawn, {CurrentWaypoint.X, CurrentWaypoint.Y});
	}
}

EBTNodeResult::Type UBTT_EnterHouse_StukalovsAlex::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Removing all the momentum to not distort the other movement input
	SurvivorPawn->GetMovementComponent()->StopMovementImmediately();
	
	return EBTNodeResult::Aborted;
}

uint16 UBTT_EnterHouse_StukalovsAlex::GetInstanceMemorySize() const
{
	// Without it, the NodeMemory will be of size 0, so any writes will result in segfault
	return sizeof(FEnterHouseMemory);
}

void UBTT_EnterHouse_StukalovsAlex::SaveHouseAsVisited() const noexcept
{
	// Adding the current house to the list of the visited ones if it is not there already
	HouseTrackerComponent->SaveHouseAsVisited(*House);
}
