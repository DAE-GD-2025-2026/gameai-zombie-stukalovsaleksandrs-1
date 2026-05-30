// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManager_StukalovsAlex.h"
#include "Common/InventoryComponent.h"

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

bool UInventoryManager_StukalovsAlex::IsInventoryFull() const
{
	return ItemCount == InventoryComponent->GetInventoryCapacity();
}

void UInventoryManager_StukalovsAlex::BeginPlay()
{
	Super::BeginPlay();

	// Accessing the inventory component of the owner
	InventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
	verify(InventoryComponent);
}
