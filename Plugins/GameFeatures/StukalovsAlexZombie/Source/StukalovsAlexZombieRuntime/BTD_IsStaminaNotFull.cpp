// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsStaminaNotFull.h"

#include "AIController.h"
#include "Survivor/SurvivorPawn.h"

UBTD_IsStaminaNotFull::UBTD_IsStaminaNotFull()
{
	NodeName = "IsStaminaNotFull";
}

bool UBTD_IsStaminaNotFull::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// Getting the survivor pawn
	APawn* Pawn{ OwnerComp.GetAIOwner()->GetPawn() };
	ASurvivorPawn* SurvivorPawn{ CastChecked<ASurvivorPawn>(Pawn) };

	// Getting the stamina component
	UStaminaComponent const * const StaminaComponent{ SurvivorPawn->GetComponentByClass<UStaminaComponent>() };
	verify(StaminaComponent);
	
	return StaminaComponent->GetCurrentStamina() < StaminaComponent->GetMaxStamina();
}
