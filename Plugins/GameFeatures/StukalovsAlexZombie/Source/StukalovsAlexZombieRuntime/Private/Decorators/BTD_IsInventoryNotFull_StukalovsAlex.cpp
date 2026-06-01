// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTD_IsInventoryNotFull_StukalovsAlex.h"

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
	ASurvivorPawn const * const SurvivorPawn{ Cast<ASurvivorPawn>(OwnerComp.GetOwner()) };
	if (not SurvivorPawn) return false;
	// Getting the inventory manager
	UInventoryManagerComponent_StukalovsAlex* const InventoryManager{ SurvivorPawn->GetComponentByClass<UInventoryManagerComponent_StukalovsAlex>() };
	verify(InventoryManager);
	// Returning if it's full
	return not InventoryManager->IsInventoryFull();
}
