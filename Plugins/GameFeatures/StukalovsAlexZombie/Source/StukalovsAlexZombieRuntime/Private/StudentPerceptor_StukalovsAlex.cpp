// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor_StukalovsAlex.h"
#include "Zombies/BaseZombie.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
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
			BlackboardComponent->SetValueAsObject(HouseKey, House);
		}

		if (ABaseItem* Item{ Cast<ABaseItem>(Actor) }; Item)
		{
			// Check if this item is more valuable 
			BlackboardComponent->SetValueAsObject(ItemKey, Item);
		}

		if (ABaseZombie* Zombie{ Cast<ABaseZombie>(Actor) }; Zombie)
		{
			BlackboardComponent->SetValueAsObject(ZombieKey, Zombie);
		}

	}

	// Just for the moment when the damaged sense gets fixed
	if (Stimulus.Type == UAISense_Damage::GetSenseID<UAISense_Damage>())
	{
		if (ABaseZombie* Zombie{ Cast<ABaseZombie>(Actor) }; Zombie)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Zombie hit character"));
			BlackboardComponent->SetValueAsObject(ZombieKey, Zombie);
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
				if (ASurvivorPawn* SurvivorPawn{ Cast<ASurvivorPawn>(GetOwner()) };
					Zombie->GetDistanceTo(SurvivorPawn) < ClosestZombie->GetDistanceTo(SurvivorPawn))
				{
					ClosestZombie = Cast<ABaseZombie>(Zombie);
				}
			}
			// Saving the closest zombie to blackboard
			BlackboardComponent->SetValueAsObject(ZombieKey, ClosestZombie);
		}
	}
}
