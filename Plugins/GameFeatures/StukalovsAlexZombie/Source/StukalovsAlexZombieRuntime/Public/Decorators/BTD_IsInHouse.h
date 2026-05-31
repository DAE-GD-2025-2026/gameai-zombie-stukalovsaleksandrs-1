// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsInHouse.generated.h"

/**
 * Whether the owner is the BT is inside a house
 */
UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTD_IsInHouse final : public UBTDecorator
{
	GENERATED_BODY()

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
private:
	FName const HouseKey{ TEXT("House") };
	
};
