// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Wander_StukalovsAlex.generated.h"

class ASurvivorPawn;

UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTT_Wander_StukalovsAlex final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTT_Wander_StukalovsAlex();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
