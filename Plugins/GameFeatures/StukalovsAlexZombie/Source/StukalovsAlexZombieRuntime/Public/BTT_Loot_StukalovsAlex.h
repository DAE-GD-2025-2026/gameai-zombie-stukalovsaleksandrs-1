// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Loot_StukalovsAlex.generated.h"

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

private:
	// Returns whether the item was successfully used
	static bool TryUsingItem(ABaseItem&, ASurvivorPawn&);
	
};
