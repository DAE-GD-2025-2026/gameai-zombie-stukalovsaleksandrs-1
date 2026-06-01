// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_Wander_StukalovsAlex.h"

#include "BTTUtils_StukalovsAlex.h"
#include "Components/SteeringComponent_StukalovsAlex.h"
#include "Survivor/SurvivorPawn.h"

UBTT_Wander_StukalovsAlex::UBTT_Wander_StukalovsAlex()
{
	NodeName = "Wander";
}

EBTNodeResult::Type UBTT_Wander_StukalovsAlex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASurvivorPawn* SurvivorPawn = BTTUtils_StukalovsAlex::GetOwner(OwnerComp);
	verify(SurvivorPawn);
	
	USteeringComponent_StukalovsAlex* SteeringComponent = SurvivorPawn->GetComponentByClass<USteeringComponent_StukalovsAlex>();
	verify(SteeringComponent);
	SteeringComponent->SetBehavior<FWander_StukalovsAlex>();
	return EBTNodeResult::InProgress;
}
