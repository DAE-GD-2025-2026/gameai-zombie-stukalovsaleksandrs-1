// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Shoot_StukalovsAlex.generated.h"

class USteeringComponent_StukalovsAlex;
class UInventoryManagerComponent_StukalovsAlex;
class APistol;
class AShotgun;
class ABaseItem;
class ASurvivorPawn;
class ABaseZombie;
class UHealthComponent;

UCLASS()
class STUKALOVSALEXZOMBIERUNTIME_API UBTT_Shoot_StukalovsAlex final : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_Shoot_StukalovsAlex();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	float MaxShotgunDistance{ 200.f };// Max distance to use shotgun(in cm by default)

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ZombieKey;

	ASurvivorPawn* SurvivorPawn{};
	UInventoryManagerComponent_StukalovsAlex* InventoryManager{};
	USteeringComponent_StukalovsAlex* SteeringComponent{};
	
	ABaseZombie* Zombie{};
	UHealthComponent* ZombieHealthComponent{};
	
	ABaseItem* CurrentWeapon{};
	AShotgun* Shotgun{};
	APistol* Pistol{};
	

	// Gets the first pistol and shotgun if available
	// Retuns false if no guns available, true otherwise
	bool SelectGuns();
	// Sets the proper weapon as current depending on weapon availability and distance from target
	void SelectProperWeapon(UBehaviorTreeComponent& OwnerComp) noexcept;
	void Shoot(UBehaviorTreeComponent& OwnerComp) const noexcept;
};
