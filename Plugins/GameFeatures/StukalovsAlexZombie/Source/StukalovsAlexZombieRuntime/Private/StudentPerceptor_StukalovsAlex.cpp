// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor_StukalovsAlex.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
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
}

void UStudentPerceptor_StukalovsAlex::OnPerceptionUpdated(AActor* Actor, FAIStimulus const Stimulus)
{
	// 1. Performing input validation
	verify(Actor);
	if (!Stimulus.WasSuccessfullySensed() || Stimulus.IsExpired()) return;
	
	// 2. Checking if we see a house
	if (Stimulus.Type == UAISense_Sight::GetSenseID<UAISense_Sight>())
	{
		// 2. Setting the house blackboard reference if the perceived actor is a house
		AHouse* HouseActor = Cast<AHouse>(Actor);
		if (HouseActor)
		{
			BlackboardComponent->SetValueAsObject(HouseKey, HouseActor);
		}
	}
}
