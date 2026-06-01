// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Flight_StukalovsAlex.generated.h"

class ABaseZombie;
class USteeringComponent_StukalovsAlex;
class ASurvivorPawn;

UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTT_Flight_StukalovsAlex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTT_Flight_StukalovsAlex();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	ASurvivorPawn* SurvivorPawn{};
	USteeringComponent_StukalovsAlex* SteeringComponent{};
	ABaseZombie* Zombie{};	
};
