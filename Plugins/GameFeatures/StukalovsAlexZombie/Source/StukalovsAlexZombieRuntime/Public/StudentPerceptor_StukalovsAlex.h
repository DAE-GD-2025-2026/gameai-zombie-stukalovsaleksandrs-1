// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "StudentPerceptor_StukalovsAlex.generated.h"

class UHouseTrackerComponent_StukalovsAlex;
class AHouse;
class UHealthComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STUKALOVSALEXZOMBIERUNTIME_API UStudentPerceptor_StukalovsAlex : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStudentPerceptor_StukalovsAlex();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Perception")
	FName HouseKeyName{ "House" };
	
	UPROPERTY(EditAnywhere, Category = "Perception")
	FName ItemKeyName{ "Item" };

	UPROPERTY(EditAnywhere, Category = "Perception")
	FBlackboardKeySelector ZombieKey;
	FName ZombieKeyName{ "Zombie" };
	
	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY()
	UBlackboardComponent* BlackboardComponent{};

	float OldHealth{};
	UHealthComponent* HealthComponent{};
	UHouseTrackerComponent_StukalovsAlex* HouseTrackerComponent{};

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	

};
