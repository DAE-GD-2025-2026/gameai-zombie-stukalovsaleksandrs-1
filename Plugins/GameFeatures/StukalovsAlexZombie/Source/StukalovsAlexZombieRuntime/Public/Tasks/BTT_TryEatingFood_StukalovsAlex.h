// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_TryEatingFood_StukalovsAlex.generated.h"

/**
 * Makes character eat the food if it is in the inventory and will not replenish more stamina than is lost
 */
UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTT_TryEatingFood_StukalovsAlex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_TryEatingFood_StukalovsAlex();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
