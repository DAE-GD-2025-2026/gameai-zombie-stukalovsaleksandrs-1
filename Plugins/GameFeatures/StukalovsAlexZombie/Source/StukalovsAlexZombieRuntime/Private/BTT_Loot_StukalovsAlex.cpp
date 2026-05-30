// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_Loot_StukalovsAlex.h"

#include "InventoryManager_StukalovsAlex.h"
#include "Common/InventoryComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Tasks/BTTUtils_StukalovsAlex.h"
#include "Village/House/House.h"

UBTT_Loot_StukalovsAlex::UBTT_Loot_StukalovsAlex()
{
	NodeName = "Loot";
}

EBTNodeResult::Type UBTT_Loot_StukalovsAlex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASurvivorPawn * const SurvivorPawn{ BTTUtils_StukalovsAlex::GetOwner(OwnerComp) };
	verify(SurvivorPawn);
	
	// Getting the pickup from the vicinity
	ABaseItem * const Item{ BTTUtils_StukalovsAlex::GetBlackboardObject<ABaseItem>(OwnerComp, TEXT("Item")) };
	verify(Item);// There's a decorator on this task checking for the existence of the item in the blackboard

	// Getting inventory components
	UInventoryComponent * const InventoryComponent{ SurvivorPawn->FindComponentByClass<UInventoryComponent>() };
	verify(InventoryComponent);
	UInventoryManager_StukalovsAlex * const InventoryManager{ SurvivorPawn->FindComponentByClass<UInventoryManager_StukalovsAlex>() };
	verify(InventoryManager);

	// Cannot pick anything up if the inventory is full
	if (InventoryManager->IsInventoryFull()) return EBTNodeResult::Failed;

	// Looting or using the item if the character can reach it
	if (float const PickupRange{ InventoryComponent->GetPickupRange() }; (Item->GetActorLocation() - SurvivorPawn->GetActorLocation()).SquaredLength() < PickupRange * PickupRange)// Item within range
	{
		if (!TryUsingItem(*Item, *SurvivorPawn))
		{
			if (!InventoryManager->TryTakingItem(*Item))
			{
				return EBTNodeResult::Failed;
			}
		}
		return EBTNodeResult::Succeeded;
	}
	
	// Item out of reach -> moving towards it
	FVector const ItemLocation{ Item->GetActorLocation()};
	BTTUtils_StukalovsAlex::SetSteeringTarget(*SurvivorPawn, {ItemLocation.X, ItemLocation.Y});

	return EBTNodeResult::Succeeded;
}

bool UBTT_Loot_StukalovsAlex::TryUsingItem(ABaseItem& Item, ASurvivorPawn& SurvivorPawn)
{
	switch (Item.GetItemType())
	{
	case EItemType::Food:
	{
		// Eating if it will refill as much stamina as it can
		UStaminaComponent* StaminaComponent{ SurvivorPawn.GetComponentByClass<UStaminaComponent>() };
		verify(StaminaComponent);
		if (Item.GetValue() <= StaminaComponent->GetMaxStamina() - StaminaComponent->GetCurrentStamina())
		{
			Item.UseItem(SurvivorPawn);
			return true;
		}
		return false;// Won't use now
	}
	case EItemType::Medkit:
	{
		// Using if it refills as much health as it can
		UHealthComponent* HealthComponent{ SurvivorPawn.GetComponentByClass<UHealthComponent>() };
		verify(HealthComponent);
		if (Item.GetValue() <= HealthComponent->GetMaxHealth() - HealthComponent->GetHealth())
		{
			Item.UseItem(SurvivorPawn);
			return true;
		}
		return false;
	}
	case EItemType::Garbage:
		Item.Destroy();
		return true;
	default: return false;// Won't try using firearms, only loot
	}
}
