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

