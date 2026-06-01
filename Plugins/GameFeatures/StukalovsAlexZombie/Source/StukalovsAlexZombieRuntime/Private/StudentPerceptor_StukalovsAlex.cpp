// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor_StukalovsAlex.h"
#include "Zombies/BaseZombie.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "SurvivorAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HouseTrackerComponent_StukalovsAlex.h"
#include "Components/SteeringComponent_StukalovsAlex.h"
#include "Items/BaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Village/House/House.h"


UStudentPerceptor_StukalovsAlex::UStudentPerceptor_StukalovsAlex()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptor_StukalovsAlex::BeginPlay()
{
	Super::BeginPlay();

	// Getting the perception component
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor_StukalovsAlex::OnPerceptionUpdated);
	}
	
	// Getting the blackboard component
	if (AAIController* AIC{ Cast<AAIController>(GetOwner()->GetInstigatorController()) })
	{
		BlackboardComponent = AIC->GetBlackboardComponent();
		verify(BlackboardComponent);
	}

	// Registering the houses to trigger the sight perception
	for (AHouse* House : TActorRange<AHouse>(GetWorld()))
	{
		UAIPerceptionStimuliSourceComponent* const StimuliSource{ House->GetComponentByClass<UAIPerceptionStimuliSourceComponent>() };
		verify(StimuliSource);
		StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());// Setting the category the sense is attributed to 
		StimuliSource->RegisterWithPerceptionSystem();// Making the stimulus tracked by the perception system
	}

	// Setting the initial health
	HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
	verify(HealthComponent);
	OldHealth = HealthComponent->GetHealth();

	// Getting the house tracker component
	HouseTrackerComponent = GetOwner()->FindComponentByClass<UHouseTrackerComponent_StukalovsAlex>();
	verify(HouseTrackerComponent);
}

// Prioritizing guns, then medkits, then food
uint32_t GetItemPriority(ABaseItem const& Item)
{
	switch (Item.GetItemType())
	{
	case EItemType::Shotgun:
		return 3;
	case EItemType::Pistol:
		return 3;
	case EItemType::Medkit:
		return 2;
	case EItemType::Food:
		return 1;		
	default: return 0;
	}
}

void UStudentPerceptor_StukalovsAlex::OnPerceptionUpdated(AActor* Actor, FAIStimulus const Stimulus)
{
	// Performing input validation
	verify(Actor);
	if (!Stimulus.WasSuccessfullySensed() || Stimulus.IsExpired()) return;

	// Processing visibility
	if (Stimulus.Type == UAISense_Sight::GetSenseID<UAISense_Sight>())
	{
		if (AHouse* House{ Cast<AHouse>(Actor) }; House)
		{
			if (HouseTrackerComponent->IsHouseVisited(*House)) return;// Ignoring the visited houses
			
			// The house is not visited...

			if (AHouse* CurrentHouse{ (Cast<AHouse>(BlackboardComponent->GetValueAsObject(HouseKeyName))) })
			{
				// Choosing the closest between this house and the currently sensed one
				House = Actor->GetDistanceTo(CurrentHouse) < Actor->GetDistanceTo(House) ? CurrentHouse : House;
			}

			// Adding the closest house to the blackboard
			BlackboardComponent->SetValueAsObject(HouseKeyName, House);
#ifdef ENABLE_DEBUG_VISUALS
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Updated closest unvisited house"));
#endif
		}

		if (ABaseItem* Item{ Cast<ABaseItem>(Actor) }; Item)
		{
			// TODO: DRY. Too similar to house perception
			// Check if this item is more valuable 
			if (ABaseItem* CurrentItem{ (Cast<ABaseItem>(BlackboardComponent->GetValueAsObject(ItemKeyName))) })
			{
				// Choosing the closest between this item and the currently sensed one
				Item = GetItemPriority(*CurrentItem) < GetItemPriority(*Item) ? CurrentItem : Item;
			}

			// Adding the closest item to the blackboard
			BlackboardComponent->SetValueAsObject(ItemKeyName, Item);
#ifdef ENABLE_DEBUG_VISUALS
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Updated closest item"));
#endif
		}

		if (ABaseZombie* Zombie{ Cast<ABaseZombie>(Actor) }; Zombie)
		{
			BlackboardComponent->SetValueAsObject(ZombieKeyName, Zombie);
		}

	}

	// Just for the moment when the damaged sense gets fixed
	if (Stimulus.Type == UAISense_Damage::GetSenseID<UAISense_Damage>())
	{
		if (ABaseZombie* Zombie{ Cast<ABaseZombie>(Actor) }; Zombie)
		{
	#ifdef ENABLE_DEBUG_VISUALIZATION
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Zombie hit character"));
	#endif
			BlackboardComponent->SetValueAsObject(ZombieKey.SelectedKeyName, Zombie);
		}
	}
}

void UStudentPerceptor_StukalovsAlex::TickComponent(float const DeltaTime, enum ELevelTick const TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HealthComponent->GetHealth() < OldHealth)// Got hit by a zombie
	{
		OldHealth = HealthComponent->GetHealth();
		// Getting the closest zombie and saving it to blackboard
		TArray<AActor*> Zombies;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseZombie::StaticClass(), Zombies);
		if(!Zombies.IsEmpty())
		{
			ABaseZombie* ClosestZombie{ Cast<ABaseZombie>(Zombies[0]) };
			for (AActor* const Zombie : Zombies)
			{
				if (ASurvivorPawn const * const SurvivorPawn{ Cast<ASurvivorPawn>(GetOwner()) };
					Zombie->GetDistanceTo(SurvivorPawn) < ClosestZombie->GetDistanceTo(SurvivorPawn))
				{
					ClosestZombie = Cast<ABaseZombie>(Zombie);
				}
			}
			// Saving the closest zombie to blackboard
			BlackboardComponent->SetValueAsObject(ZombieKey.SelectedKeyName, ClosestZombie);
		}
	}
}
