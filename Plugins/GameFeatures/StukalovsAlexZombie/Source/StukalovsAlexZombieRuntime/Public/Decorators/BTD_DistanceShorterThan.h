// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTD_DistanceShorterThan.generated.h"

/**
 * 
 */
UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTD_DistanceShorterThan : public UBTDecorator_Blackboard
{
	GENERATED_BODY()
public:
	UBTD_DistanceShorterThan();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	float MaxDistance{ 100 };
};
