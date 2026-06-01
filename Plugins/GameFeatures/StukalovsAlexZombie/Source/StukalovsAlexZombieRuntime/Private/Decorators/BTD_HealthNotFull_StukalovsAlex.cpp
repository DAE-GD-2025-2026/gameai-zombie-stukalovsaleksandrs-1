// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTD_HealthNotFull_StukalovsAlex.h"

#include "AIController.h"
#include "Survivor/SurvivorPawn.h"

UBTD_HealthNotFull_StukalovsAlex::UBTD_HealthNotFull_StukalovsAlex()
{
	NodeName = "IsHealthNotFull";
}

bool UBTD_HealthNotFull_StukalovsAlex::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// Getting the survivor pawn
	APawn* Pawn{ OwnerComp.GetAIOwner()->GetPawn() };
	ASurvivorPawn* SurvivorPawn{ CastChecked<ASurvivorPawn>(Pawn) };

	// Seeing if the health is full
	UHealthComponent const * const HealthComponent{ SurvivorPawn->GetComponentByClass<UHealthComponent>() };
	verify(HealthComponent);

	return HealthComponent->GetHealth() < HealthComponent->GetMaxHealth();
}
