// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_EnterHouse_StukalovsAlex.generated.h"

class ASurvivorPawn;

UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTT_EnterHouse_StukalovsAlex final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_EnterHouse_StukalovsAlex();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override;
	
private:
	UPROPERTY()
	float WaypointAcceptanceRadius{ 50.f };

	[[nodiscard]] static ASurvivorPawn* GetOwner(UBehaviorTreeComponent& OwnerComp) noexcept;
};
