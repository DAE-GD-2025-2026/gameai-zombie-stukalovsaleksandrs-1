// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTD_DistanceShorterThan.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"

UBTD_DistanceShorterThan::UBTD_DistanceShorterThan()
{
	NodeName = "DistanceShorterThan";
}

bool UBTD_DistanceShorterThan::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// Getting the survivor pawn
	APawn* Pawn{ OwnerComp.GetAIOwner()->GetPawn() };
	ASurvivorPawn* SurvivorPawn{ CastChecked<ASurvivorPawn>(Pawn) };

	UBlackboardComponent* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	verify(BlackboardComponent);
	UObject* TargetObject{ BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName) };
	AActor* TargetActor{ CastChecked<AActor>(TargetObject) };
	return (TargetActor->GetActorLocation() - SurvivorPawn->GetActorLocation()).SizeSquared() < MaxDistance * MaxDistance;
}
