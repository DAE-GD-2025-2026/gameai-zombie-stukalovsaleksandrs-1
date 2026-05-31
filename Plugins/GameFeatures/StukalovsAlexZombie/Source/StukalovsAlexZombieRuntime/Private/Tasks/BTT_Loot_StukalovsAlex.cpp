// Fill out your copyright notice in the Description page of Project Settings.

// Plugin
#include "Tasks/BTT_Loot_StukalovsAlex.h"
#include "Tasks/BTTUtils_StukalovsAlex.h"
#include "InventoryManager_StukalovsAlex.h"
// Project
#include "Common/InventoryComponent.h"
#include "SteeringBehaviors/SteeringComponent_StukalovsAlex.h"
#include "Survivor/SurvivorPawn.h"

UBTT_Loot_StukalovsAlex::UBTT_Loot_StukalovsAlex()
{
	bNotifyTick = true;
	NodeName = "Loot";
}

EBTNodeResult::Type UBTT_Loot_StukalovsAlex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Getting the pickup from the vicinity
	ABaseItem * const Item{ BTTUtils_StukalovsAlex::GetBlackboardObject<ABaseItem>(OwnerComp, TEXT("Item")) };
	TryPushingItem(Item);
	if (ItemsToLoot.empty()) return EBTNodeResult::Failed;

	// Setting up character data
	SurvivorPawn = BTTUtils_StukalovsAlex::GetOwner(OwnerComp);
	verify(SurvivorPawn);
	SteeringComponent = SurvivorPawn->GetComponentByClass<USteeringComponent_StukalovsAlex>();
	verify(SteeringComponent);
	SteeringComponent->SetBehavior<FSeek_StukalovsAlex>();
	
	// Getting inventory components

	InventoryManager = SurvivorPawn->FindComponentByClass<UInventoryManager_StukalovsAlex>();
	verify(InventoryManager);

	return EBTNodeResult::InProgress;
}

void UBTT_Loot_StukalovsAlex::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// Cannot pick anything up if the inventory is full
	if (InventoryManager->IsInventoryFull())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (ItemsToLoot.empty())// Looted all items
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// Getting the next item to loot
	ABaseItem* const Item{ ItemsToLoot.back() };
	
	// Looting or using the item if the character can reach it
	if (float const PickupRange{ InventoryManager->GetPickupRange() }; (Item->GetActorLocation() - SurvivorPawn->GetActorLocation()).SquaredLength() < PickupRange * PickupRange)// Item within range
	{
		if (!TryUsingItem(*Item, *SurvivorPawn))
		{
			if (!InventoryManager->TryTakingItem(*Item))
			{
				ItemsToLoot.clear();
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}
		}

		ItemsToLoot.pop_back();
		// return EBTNodeResult::Succeeded;
	}
	else// Item out of reach -> moving towards it
	{
		MoveToItem(*Item);		
	}
}

void UBTT_Loot_StukalovsAlex::TryPushingItem(ABaseItem* Item) noexcept
{
	if (!Item) return;
	if (std::ranges::find(ItemsToLoot, Item) != ItemsToLoot.end())// Item exists
	{
		return;// Not adding the item
	}
	ItemsToLoot.push_back(Item);// Adding the item
}

void UBTT_Loot_StukalovsAlex::MoveToItem(ABaseItem const& Item) noexcept
{
	// TArray Path{ SurvivorPawn->CalculatePath(Item.GetActorLocation()) };
	// verify(!Path.IsEmpty());
	FVector const ItemLocation{ Item.GetActorLocation() };
	SteeringComponent->SetTarget({ItemLocation.X, ItemLocation.Y});
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
