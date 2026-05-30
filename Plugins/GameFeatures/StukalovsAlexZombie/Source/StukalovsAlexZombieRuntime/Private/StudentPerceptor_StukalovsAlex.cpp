// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor_StukalovsAlex.h"

#include "AIController.h"
#include "ContentBrowserItem.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/BaseItem.h"
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
	// Performing input validation
	verify(Actor);
	if (!Stimulus.WasSuccessfullySensed() || Stimulus.IsExpired()) return;

	// Processing visibility
	if (Stimulus.Type == UAISense_Sight::GetSenseID<UAISense_Sight>())
	{
		if (AHouse* House{ Cast<AHouse>(Actor) }; House)
		{
			BlackboardComponent->SetValueAsObject(HouseKey, House);
			return;
		}

		if (ABaseItem* Item{ Cast<ABaseItem>(Actor) }; Item)
		{
			// Check if this item is more valuable 
			BlackboardComponent->SetValueAsObject(ItemKey, Item);
			return;
		}
	}

}
