// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsStaminaNotFull.h"

#include "Survivor/SurvivorPawn.h"

UBTD_IsStaminaNotFull::UBTD_IsStaminaNotFull()
{
	NodeName = "IsStaminaNotFull";
}

bool UBTD_IsStaminaNotFull::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// Getting the survivor
	ASurvivorPawn const * const SurvivorPawn{ Cast<ASurvivorPawn>(OwnerComp.GetOwner()) };
	verify(SurvivorPawn);

	// Getting the stamina component
	UStaminaComponent const * const StaminaComponent{ SurvivorPawn->GetComponentByClass<UStaminaComponent>() };
	verify(StaminaComponent);
	
	return StaminaComponent->GetCurrentStamina() < StaminaComponent->GetMaxStamina();
}
