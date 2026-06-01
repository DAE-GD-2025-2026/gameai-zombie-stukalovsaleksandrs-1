// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_LookAround_StukalovsAlex.generated.h"


class UHouseTrackerComponent_StukalovsAlex;
class USteeringComponent_StukalovsAlex;
class ASurvivorPawn;
// Rotates smoothly by -AbsDegToTurn and then by +AbsDegToTurn 
UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTT_LookAround_StukalovsAlex final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_LookAround_StukalovsAlex();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere)
	float AbsDegToTurn{ 150.f };
	UPROPERTY(EditAnywhere)
	float DegPerSec{ 120.f };
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ShouldLookAroundKey;

	float StartYaw{};
	float TargetYaw{};
	float TurnDirection{ 1.f };
	enum class ETurningPhase{ Right, Left, BackToStart } Phase{};
    
	ASurvivorPawn* SurvivorPawn{};
	USteeringComponent_StukalovsAlex* SteeringComponent{};
	UHouseTrackerComponent_StukalovsAlex* HouseTrackerComponent{};

};
