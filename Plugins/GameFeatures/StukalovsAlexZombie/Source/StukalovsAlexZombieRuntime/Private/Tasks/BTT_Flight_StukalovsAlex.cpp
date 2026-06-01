// Fill out your copyright notice in the Description page of Project Settings.

#include "Tasks/BTT_Flight_StukalovsAlex.h"

#include "BTTUtils_StukalovsAlex.h"
#include "Components/SteeringComponent_StukalovsAlex.h"
#include "Survivor/SurvivorPawn.h"
#include "Zombies/BaseZombie.h"

UBTT_Flight_StukalovsAlex::UBTT_Flight_StukalovsAlex()
{
	bNotifyTick = false;
	NodeName = "Flight";
}

EBTNodeResult::Type UBTT_Flight_StukalovsAlex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	SurvivorPawn = BTTUtils_StukalovsAlex::GetOwner(OwnerComp);
	verify(SurvivorPawn);
	
	SteeringComponent = SurvivorPawn->GetComponentByClass<USteeringComponent_StukalovsAlex>();
	verify(SteeringComponent);
	SteeringComponent->SetBehavior<FFlight_StukalovsAlex>();

	Zombie = BTTUtils_StukalovsAlex::GetBlackboardObject<ABaseZombie>(OwnerComp, TEXT("Zombie"));
	if (!Zombie)
	{
		return EBTNodeResult::Failed;
	}
	
	FVector const ZombieLocation{ Zombie->GetActorLocation() };
	SteeringComponent->SetTarget({ZombieLocation.X, ZombieLocation.Y});
	
	return EBTNodeResult::Succeeded;
}
