// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_Chase_StukalovsAlex.h"

#include "BTTUtils_StukalovsAlex.h"
#include "SteeringBehaviors/SteeringComponent_StukalovsAlex.h"
#include "Survivor/SurvivorPawn.h"
#include "Zombies/BaseZombie.h"

UBTT_Chase_StukalovsAlex::UBTT_Chase_StukalovsAlex()
{
	bNotifyTick = true;
	NodeName = "Chase";
}

EBTNodeResult::Type UBTT_Chase_StukalovsAlex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	SurvivorPawn = BTTUtils_StukalovsAlex::GetOwner(OwnerComp);
	verify(SurvivorPawn);
	
	SteeringComponent = SurvivorPawn->GetComponentByClass<USteeringComponent_StukalovsAlex>();
	verify(SteeringComponent);
	SteeringComponent->SetBehavior<FSeek_StukalovsAlex>();

	Zombie = BTTUtils_StukalovsAlex::GetBlackboardObject<ABaseZombie>(OwnerComp, TEXT("Zombie"));
	verify(Zombie);
	
	return EBTNodeResult::InProgress;
}

void UBTT_Chase_StukalovsAlex::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector const ZombieLocation{ Zombie->GetActorLocation() };
	SteeringComponent->SetTarget({ZombieLocation.X, ZombieLocation.Y});
}
