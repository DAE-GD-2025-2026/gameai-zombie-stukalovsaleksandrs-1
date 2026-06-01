// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Loot_StukalovsAlex.generated.h"

class USteeringComponent_StukalovsAlex;
class UInventoryComponent;
class ASurvivorPawn;
class ABaseItem;
class UInventoryManager_StukalovsAlex;
/**
 * Task for looting items in a game world
 */
UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTT_Loot_StukalovsAlex final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTT_Loot_StukalovsAlex();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	// Returns whether the item was successfully used
	ASurvivorPawn* SurvivorPawn{};
	USteeringComponent_StukalovsAlex* SteeringComponent{};
	std::vector<ABaseItem*> ItemsToLoot;// not std::stack, bc I want to use std::find

	UInventoryManager_StukalovsAlex* InventoryManager{};
	
	static bool TryUsingItem(ABaseItem&, ASurvivorPawn&);
	void TryPushingItem(ABaseItem* Item) noexcept;
	void MoveToItem(ABaseItem const& Item) noexcept;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ShouldLookAroundKey;
};
