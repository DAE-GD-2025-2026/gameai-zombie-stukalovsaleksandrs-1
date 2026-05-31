// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/InventoryComponent.h"
#include "InventoryManager_StukalovsAlex.generated.h"

class ABaseItem;
// A wrapper over the UInventoryComponent that manages the slots
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STUKALOVSALEXZOMBIERUNTIME_API UInventoryManager_StukalovsAlex : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryManager_StukalovsAlex();

	// Returns whether an item was successfully taken
	bool TryTakingItem(ABaseItem& Item);

	[[nodiscard]] bool IsInventoryFull() const noexcept;

	void RemoveValuelessElements() noexcept;

	[[nodiscard]] float GetPickupRange() const noexcept;

protected:
	virtual void BeginPlay() override;
	
private:
	UInventoryComponent* InventoryComponent{};
	uint32_t ItemCount{};
};
