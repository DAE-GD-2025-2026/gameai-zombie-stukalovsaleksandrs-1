// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_LookAround_StukalovsAlex.h"

#include "BTTUtils_StukalovsAlex.h"
#include "Survivor/SurvivorPawn.h"

UBTT_LookAround_StukalovsAlex::UBTT_LookAround_StukalovsAlex()
{
	bNotifyTick = true;
	NodeName = "LookAround";
}

EBTNodeResult::Type UBTT_LookAround_StukalovsAlex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	SurvivorPawn = BTTUtils_StukalovsAlex::GetOwner(OwnerComp);
	verify(SurvivorPawn);

	StartYaw = SurvivorPawn->GetActorRotation().Yaw;
	Phase = ETurningPhase::Right;
	TargetYaw = AbsDegToTurn;

	return EBTNodeResult::InProgress;
}

void UBTT_LookAround_StukalovsAlex::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float const DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	float const CurrentYaw{ static_cast<float>(SurvivorPawn->GetActorRotation().Yaw) };
	float const Remaining{ static_cast<float>(FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw))) };

	if (Remaining < 2.f)
	{
		switch (Phase)
		{
		case ETurningPhase::Right:
			Phase = ETurningPhase::Left;
			TargetYaw = StartYaw - AbsDegToTurn;
			TurnDirection = -1.f; // Force turning left (through 0)
			break;
		case ETurningPhase::Left:
			Phase = ETurningPhase::BackToStart;
			TargetYaw = StartYaw;
			TurnDirection = 1.f; // Force turning right back to start
			break;
		case ETurningPhase::BackToStart:
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		default: ;
		}
	}

	// Step in the explicit direction instead of interpolating
	float const Step{ DegPerSec * DeltaSeconds * TurnDirection };
	float const NewYaw{ CurrentYaw + Step };
	SurvivorPawn->SetActorRotation(FRotator{ 0.f, NewYaw, 0.f });
}
