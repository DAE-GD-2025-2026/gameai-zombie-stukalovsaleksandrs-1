#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "BTTUtils_StukalovsAlex.h"

#include "SteeringBehaviors/SteeringComponent_StukalovsAlex.h"

ASurvivorPawn* BTTUtils_StukalovsAlex::GetOwner(UBehaviorTreeComponent& OwnerComp) noexcept
{
	AAIController* const Controller = OwnerComp.GetAIOwner();
	if (!Controller) return nullptr;
	
	ASurvivorPawn* const SurvivorPawn = Cast<ASurvivorPawn>(Controller->GetPawn());
	if (!SurvivorPawn) return nullptr;
	
	return SurvivorPawn;
}

void BTTUtils_StukalovsAlex::SetSteeringTarget(ASurvivorPawn& SurvivorPawn, FVector2D NewTarget)
{
	USteeringComponent_StukalovsAlex* SteeringBehaviorComponent{
		SurvivorPawn.GetComponentByClass<USteeringComponent_StukalovsAlex>()
	};
	verify(SteeringBehaviorComponent);
	// if (!SteeringBehaviorComponent)
	// {
	// 	USteeringComponent_StukalovsAlex* const NewSteeringBehaviorComponent{
	// 		SurvivorPawn.GetWorld()->SpawnActor<USteeringComponent_StukalovsAlex>()
	// 	};
	// 	verify(NewSteeringBehaviorComponent);
	// 	NewSteeringBehaviorComponent->RegisterComponent();
	// }
	SteeringBehaviorComponent->SetTarget(NewTarget);
}

bool BTTUtils_StukalovsAlex::IsPointInHouse(FVector const& Point, FHouseBounds const& Bounds) noexcept
{
	double const MinX{ Bounds.Origin.X - Bounds.Extent.X };
	double const MaxX{ Bounds.Origin.X + Bounds.Extent.X };
	double const MinY{ Bounds.Origin.Y - Bounds.Extent.Y };
	double const MaxY{ Bounds.Origin.Y + Bounds.Extent.Y };
    
	return (Point.X >= MinX && Point.X <= MaxX && 
			Point.Y >= MinY && Point.Y <= MaxY);
}

