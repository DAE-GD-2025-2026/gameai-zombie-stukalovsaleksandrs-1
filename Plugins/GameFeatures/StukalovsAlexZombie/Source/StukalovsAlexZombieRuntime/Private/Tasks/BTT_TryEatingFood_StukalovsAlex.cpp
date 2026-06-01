// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_TryEatingFood_StukalovsAlex.h"

#include "BTTUtils_StukalovsAlex.h"
#include "Common/StaminaComponent.h"
#include "Components/InventoryManagerComponent_StukalovsAlex.h"
#include "Items/Food.h"
#include "Items/Medkit.h"
#include "Survivor/SurvivorPawn.h"

UBTT_TryEatingFood_StukalovsAlex::UBTT_TryEatingFood_StukalovsAlex()
{
	NodeName = "TryEatingFood";
}

EBTNodeResult::Type UBTT_TryEatingFood_StukalovsAlex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASurvivorPawn* SurvivorPawn{ BTTUtils_StukalovsAlex::GetOwner(OwnerComp) };
	verify(SurvivorPawn);
	UStaminaComponent* StaminaComponent{ SurvivorPawn->GetComponentByClass<UStaminaComponent>() };
	verify(StaminaComponent);
	UInventoryManagerComponent_StukalovsAlex* InventoryManager{ SurvivorPawn->GetComponentByClass<UInventoryManagerComponent_StukalovsAlex>() };
	verify(InventoryManager);

	TArray const FoodItems{ InventoryManager->GetItemsOfType<AFood>() };

	for (AFood* FoodItem : FoodItems)
	{
		// Trying to use the one the medkit that will not replenish more than the damage
		if (FoodItem->GetValue() <= StaminaComponent->GetMaxStamina() - StaminaComponent->GetCurrentStamina())
		{
			FoodItem->UseItem(*SurvivorPawn);
			InventoryManager->RemoveValuelessElements();
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
