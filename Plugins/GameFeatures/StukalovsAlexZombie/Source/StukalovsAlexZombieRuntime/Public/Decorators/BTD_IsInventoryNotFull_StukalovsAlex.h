// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTD_IsInventoryNotFull_StukalovsAlex.generated.h"

/**
 * Is inventory holding the max number of items it can hold?
 */
UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTD_IsInventoryNotFull_StukalovsAlex final : public UBTDecorator_Blackboard
{
	GENERATED_BODY()

public:
	UBTD_IsInventoryNotFull_StukalovsAlex() noexcept;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
