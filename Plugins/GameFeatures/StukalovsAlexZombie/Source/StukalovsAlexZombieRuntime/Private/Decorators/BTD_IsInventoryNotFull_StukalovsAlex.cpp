// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTD_IsInventoryNotFull_StukalovsAlex.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/InventoryManagerComponent_StukalovsAlex.h"
#include "Survivor/SurvivorPawn.h"

UBTD_IsInventoryNotFull_StukalovsAlex::UBTD_IsInventoryNotFull_StukalovsAlex() noexcept
{
	NodeName = "IsInventoryNotFull";
}

bool UBTD_IsInventoryNotFull_StukalovsAlex::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                    uint8* NodeMemory) const
{
	// Getting the survivor
	UBlackboardComponent* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	verify(BlackboardComponent);
	// Getting the survivor pawn
	APawn* Pawn{ OwnerComp.GetAIOwner()->GetPawn() };
	ASurvivorPawn* SurvivorPawn{ CastChecked<ASurvivorPawn>(Pawn) };
	// Getting the inventory manager
	UInventoryManagerComponent_StukalovsAlex* const InventoryManager{ SurvivorPawn->GetComponentByClass<UInventoryManagerComponent_StukalovsAlex>() };
	verify(InventoryManager);
	// Returning if it's full
	return not InventoryManager->IsInventoryFull();
}
