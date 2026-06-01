// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsStaminaNotFull.generated.h"

/**
 * 
 */
UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTD_IsStaminaNotFull : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_IsStaminaNotFull();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
