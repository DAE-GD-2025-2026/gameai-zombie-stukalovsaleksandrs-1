// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_TryUsingMedkit_StukalovsAlex.generated.h"

/**
 * 
 */
UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTT_TryUsingMedkit_StukalovsAlex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTT_TryUsingMedkit_StukalovsAlex();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
