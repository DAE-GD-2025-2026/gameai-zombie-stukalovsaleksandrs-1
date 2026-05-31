// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Chase_StukalovsAlex.generated.h"

class ABaseZombie;
class USteeringComponent_StukalovsAlex;
class ASurvivorPawn;
// Moves towards the enemy
UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTT_Chase_StukalovsAlex final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTT_Chase_StukalovsAlex();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	ASurvivorPawn* SurvivorPawn{};
	USteeringComponent_StukalovsAlex* SteeringComponent{};
	ABaseZombie* Zombie{};
};
