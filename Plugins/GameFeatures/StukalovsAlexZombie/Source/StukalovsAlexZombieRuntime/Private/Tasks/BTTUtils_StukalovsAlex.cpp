#include "BTTUtils_StukalovsAlex.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Survivor/SurvivorPawn.h"

#include "Components/InventoryManagerComponent_StukalovsAlex.h"
#include "Components/SteeringComponent_StukalovsAlex.h"

ASurvivorPawn* BTTUtils_StukalovsAlex::GetOwner(UBehaviorTreeComponent& OwnerComp) noexcept
{
	AAIController* const Controller = OwnerComp.GetAIOwner();
	if (!Controller) return nullptr;
	
	ASurvivorPawn* const SurvivorPawn = Cast<ASurvivorPawn>(Controller->GetPawn());
	if (!SurvivorPawn) return nullptr;
	
	return SurvivorPawn;
}

UBlackboardComponent& BTTUtils_StukalovsAlex::GetBlackboard(UBehaviorTreeComponent& OwnerComp) noexcept
{
	UBlackboardComponent* const BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	verify(BlackboardComponent);
	return *BlackboardComponent;
}

void BTTUtils_StukalovsAlex::SetSteeringTarget(ASurvivorPawn& SurvivorPawn, FVector2D NewTarget) noexcept
{
	USteeringComponent_StukalovsAlex* SteeringBehaviorComponent{
		SurvivorPawn.GetComponentByClass<USteeringComponent_StukalovsAlex>()
	};
	verify(SteeringBehaviorComponent);
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

TArray<ABaseItem*> BTTUtils_StukalovsAlex::GetInventory(ASurvivorPawn const& SurvivorPawn) noexcept
{
	// Getting inventory component
	UInventoryComponent * const InventoryComponent{ SurvivorPawn.FindComponentByClass<UInventoryComponent>() };
	verify(InventoryComponent);
	// Getting the items
	return InventoryComponent->GetInventory();
}

