// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_Shoot_StukalovsAlex.h"

#include "BTTUtils_StukalovsAlex.h"
#include "Items/Pistol.h"
#include "Items/Shotgun.h"
#include "Components/SteeringComponent_StukalovsAlex.h"
#include "Components/InventoryManagerComponent_StukalovsAlex.h"
#include "SteeringBehaviors/SteeringBehaviors_StukalovsAlex.h"
#include "Zombies/BaseZombie.h"

UBTT_Shoot_StukalovsAlex::UBTT_Shoot_StukalovsAlex()
{
	bNotifyTick = true;
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTT_Shoot_StukalovsAlex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Getting the zombie
	Zombie = BTTUtils_StukalovsAlex::GetBlackboardObject<ABaseZombie>(OwnerComp, TEXT("Zombie"));
	verify(Zombie)
	ZombieHealthComponent = Zombie->GetComponentByClass<UHealthComponent>();
	verify(ZombieHealthComponent);
	
	// Getting the owner
	SurvivorPawn = BTTUtils_StukalovsAlex::GetOwner(OwnerComp);
	verify(SurvivorPawn);
	
	SteeringComponent = SurvivorPawn->GetComponentByClass<USteeringComponent_StukalovsAlex>();
	verify(SteeringComponent);
	SteeringComponent->SetBehavior<FLookAt_StukalovsAlex>();

	// Getting the inventory manager(used to remove guns after running out of ammo)
	InventoryManager = SurvivorPawn->GetComponentByClass<UInventoryManagerComponent_StukalovsAlex>();
	verify(InventoryManager);

	// Getting the guns
	UpdateGuns(OwnerComp);

	// Letting the tick run
	return EBTNodeResult::InProgress;
}

void UBTT_Shoot_StukalovsAlex::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float const DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FVector const ZombieLocation{ Zombie->GetActorLocation() };
	SteeringComponent->SetTarget({ZombieLocation.X, ZombieLocation.Y});
	
	SelectProperWeapon(OwnerComp);
	
	Shoot(OwnerComp);

	InventoryManager->RemoveValuelessElements();

	UpdateGuns(OwnerComp);
}

void UBTT_Shoot_StukalovsAlex::UpdateGuns(UBehaviorTreeComponent& OwnerComp)
{
	Shotgun = nullptr;
	Pistol = nullptr;
	for (TArray const Items{ BTTUtils_StukalovsAlex::GetInventory(*SurvivorPawn) }; ABaseItem* Item : Items)
	{
		if (not Shotgun)
		{
			Shotgun = Cast<AShotgun>(Item);
			if (Shotgun) continue;
		}
		
		if (not Pistol) Pistol = Cast<APistol>(Item);

		if (Shotgun and Pistol) break;
	}
	
	// No guns left
	if (not (Pistol or Shotgun))
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
	}
}

void UBTT_Shoot_StukalovsAlex::SelectProperWeapon(UBehaviorTreeComponent& OwnerComp) noexcept
{
	// Selecting the weapon to use
	CurrentWeapon = Pistol;

	// Shotgun is available and we are within distance
	FVector SurvivorToZombie{ Zombie->GetActorLocation() - SurvivorPawn->GetActorLocation() };
	SurvivorToZombie.Normalize();
	if (Shotgun and (Zombie->GetActorLocation() - SurvivorPawn->GetActorLocation()).SquaredLength() < MaxShotgunDistance * MaxShotgunDistance)
	{
		CurrentWeapon = Shotgun;
	}

	// Pistol not available, but shotgun is
	if (not Pistol and Shotgun) CurrentWeapon = Shotgun;

	// No guns left
	if (not (Pistol or Shotgun))
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
	}
}

void UBTT_Shoot_StukalovsAlex::Shoot(UBehaviorTreeComponent& OwnerComp) const noexcept
{
	CurrentWeapon->UseItem(*SurvivorPawn);// Shooting
}
