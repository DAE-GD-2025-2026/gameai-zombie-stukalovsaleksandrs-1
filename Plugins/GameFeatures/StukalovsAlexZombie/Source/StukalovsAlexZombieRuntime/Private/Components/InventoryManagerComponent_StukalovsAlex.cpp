// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InventoryManagerComponent_StukalovsAlex.h"
#include "Common/InventoryComponent.h"
// Standard
#include <ranges>

#include "Items/Shotgun.h"
#include "Items/Weapon.h"

UInventoryManagerComponent_StukalovsAlex::UInventoryManagerComponent_StukalovsAlex()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryManagerComponent_StukalovsAlex::TryTakingItem(ABaseItem& Item) 
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
#ifdef ENABLE_DEBUG_VISUALIZATION
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Took item"));
#endif
	++ItemCount;
	return true;
}

bool UInventoryManagerComponent_StukalovsAlex::IsInventoryFull() const noexcept
{
	return ItemCount == InventoryComponent->GetInventoryCapacity();
}

void UInventoryManagerComponent_StukalovsAlex::RemoveValuelessElements() noexcept
{
	UInventoryComponent* Inventory = InventoryComponent;
	if (!Inventory) return;

	auto Items{ InventoryComponent->GetInventory() };

	// NOTE: Iterating in reverse order to avoid index-shifting issues
	for (int32 const ItemIdx : std::ranges::views::iota(0, Inventory->GetInventoryCapacity()) | std::views::reverse)
	{
		if (ABaseItem const* const Item{ Items[ItemIdx] }; Item and Item->GetValue() == 0)
		{
			// Why weapons have values of 0?
			// if (AWeapon const* const Weapon{ Cast<AWeapon>(Item) }; !Weapon)// Not throwing away weapons
			{
				Inventory->RemoveItem(ItemIdx);
				--ItemCount;
			}
		}
	}
}

float UInventoryManagerComponent_StukalovsAlex::GetPickupRange() const noexcept
{
	return InventoryComponent->GetPickupRange();
}

void UInventoryManagerComponent_StukalovsAlex::BeginPlay()
{
	Super::BeginPlay();

	// Accessing the inventory component of the owner
	InventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
	verify(InventoryComponent);
}
