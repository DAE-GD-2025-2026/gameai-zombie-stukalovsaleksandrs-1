// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManager_StukalovsAlex.h"
#include "Common/InventoryComponent.h"
// Standard
#include <ranges>
#include <vector>

UInventoryManager_StukalovsAlex::UInventoryManager_StukalovsAlex()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryManager_StukalovsAlex::TryTakingItem(ABaseItem& Item) 
{
	if (IsInventoryFull()) return false;
	if (InventoryComponent->GetInventory().Contains(&Item)) return false;
	uint32_t SlotIdx{};
	while (!InventoryComponent->GrabItem(SlotIdx, &Item))
	{
		// Could not insert at the current slot -> trying the next one
		// NOTE: Will be < capacity, bc of the check above
		++SlotIdx;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Took item"));
	++ItemCount;
	return true;
}

bool UInventoryManager_StukalovsAlex::IsInventoryFull() const noexcept
{
	return ItemCount == InventoryComponent->GetInventoryCapacity();
}

void UInventoryManager_StukalovsAlex::RemoveValuelessElements() noexcept
{
	UInventoryComponent* Inventory = InventoryComponent;
	if (!Inventory) return;

	auto Items{ InventoryComponent->GetInventory() };

	// NOTE: Iterating in reverse order to avoid index-shifting issues
	for (int32 const ItemIdx : std::ranges::views::iota(0, Inventory->GetInventoryCapacity()) | std::views::reverse)
	{
		if (ABaseItem const* const Item = Items[ItemIdx]; Item and Item->GetValue() == 0)
		{
			Inventory->RemoveItem(ItemIdx);
			--ItemCount;
		}
	}
}

float UInventoryManager_StukalovsAlex::GetPickupRange() const noexcept
{
	return InventoryComponent->GetPickupRange();
}

void UInventoryManager_StukalovsAlex::BeginPlay()
{
	Super::BeginPlay();

	// Accessing the inventory component of the owner
	InventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
	verify(InventoryComponent);
}
