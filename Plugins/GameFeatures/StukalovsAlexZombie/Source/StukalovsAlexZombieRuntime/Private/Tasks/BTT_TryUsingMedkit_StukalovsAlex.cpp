// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_TryUsingMedkit_StukalovsAlex.h"

#include "BTTUtils_StukalovsAlex.h"
#include "Components/InventoryManagerComponent_StukalovsAlex.h"
#include "Items/Medkit.h"
#include "Survivor/SurvivorPawn.h"

UBTT_TryUsingMedkit_StukalovsAlex::UBTT_TryUsingMedkit_StukalovsAlex()
{
	NodeName = "TryUsingMedkit";
}

EBTNodeResult::Type UBTT_TryUsingMedkit_StukalovsAlex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASurvivorPawn* SurvivorPawn{ BTTUtils_StukalovsAlex::GetOwner(OwnerComp) };
	verify(SurvivorPawn);
	UHealthComponent* HealthComponent{ SurvivorPawn->GetComponentByClass<UHealthComponent>() };
	verify(HealthComponent);
	UInventoryManagerComponent_StukalovsAlex* InventoryManager{ SurvivorPawn->GetComponentByClass<UInventoryManagerComponent_StukalovsAlex>() };
	verify(InventoryManager);

	TArray Medkits{ InventoryManager->GetItemsOfType<AMedkit>() };

	for (AMedkit* Medkit : Medkits)
	{
		// Trying to use the one the medkit that will not replenish more than the damage
		if (Medkit->GetValue() <= HealthComponent->GetMaxHealth() - HealthComponent->GetHealth())
		{
			Medkit->UseItem(*SurvivorPawn);
			InventoryManager->RemoveValuelessElements();
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
