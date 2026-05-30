// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "StudentPerceptor_StukalovsAlex.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STUKALOVSALEXZOMBIERUNTIME_API UStudentPerceptor_StukalovsAlex : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStudentPerceptor_StukalovsAlex();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Perception")
	FName HouseKey{ TEXT("House") };
	
	UPROPERTY(EditDefaultsOnly, Category = "Perception")
	FName ItemKey{ TEXT("Item") };
	
	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY()
	UBlackboardComponent* BlackboardComponent{};
	
};
