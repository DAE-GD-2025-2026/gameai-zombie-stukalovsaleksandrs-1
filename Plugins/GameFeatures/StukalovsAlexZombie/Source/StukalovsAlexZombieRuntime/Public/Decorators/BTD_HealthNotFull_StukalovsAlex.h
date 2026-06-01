// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTD_HealthNotFull_StukalovsAlex.generated.h"

/**
 * 
 */
UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTD_HealthNotFull_StukalovsAlex : public UBTDecorator_Blackboard
{
	GENERATED_BODY()

public:
	UBTD_HealthNotFull_StukalovsAlex();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
