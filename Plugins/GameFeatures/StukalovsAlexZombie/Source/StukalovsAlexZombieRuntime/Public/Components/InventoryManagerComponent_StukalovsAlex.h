// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/InventoryComponent.h"
#include "InventoryManagerComponent_StukalovsAlex.generated.h"

class ABaseItem;
// A wrapper over the UInventoryComponent that manages the slots
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STUKALOVSALEXZOMBIERUNTIME_API UInventoryManagerComponent_StukalovsAlex : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryManagerComponent_StukalovsAlex();

	// Returns whether an item was successfully taken
	bool TryTakingItem(ABaseItem& Item);

	[[nodiscard]] bool IsInventoryFull() const noexcept;

	void RemoveValuelessElements() noexcept;

	[[nodiscard]] float GetPickupRange() const noexcept;

	template <typename T> requires std::derived_from<T, ABaseItem>
	[[nodiscard]] TArray<T*> GetItemsOfType() noexcept
	{
		TArray Items{ InventoryComponent->GetInventory() };
		TArray<T*> Result;
    
		for (ABaseItem* Item : Items)
		{
			if (T* CastedItem = Cast<T>(Item))
			{
				Result.Add(CastedItem);
			}
		}
    
		return Result;// Empty array if nothing is found
	}

protected:
	virtual void BeginPlay() override;
	
private:
	UInventoryComponent* InventoryComponent{};
	uint32_t ItemCount{};
};
