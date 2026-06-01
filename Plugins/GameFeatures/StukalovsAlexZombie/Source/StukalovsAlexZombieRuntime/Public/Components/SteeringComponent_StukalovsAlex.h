// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SteeringBehaviors/SteeringBehaviors_StukalovsAlex.h"
#include "SteeringComponent_StukalovsAlex.generated.h"

class ASurvivorPawn;
class FSteeringBehaviorBase_StukalovsAlex;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STUKALOVSALEXZOMBIERUNTIME_API USteeringComponent_StukalovsAlex : public UActorComponent
{
	GENERATED_BODY()

public:
	USteeringComponent_StukalovsAlex();

	virtual void TickComponent(float DeltaSec, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	
	template<typename T> requires std::derived_from<T, FSteeringBehaviorBase_StukalovsAlex>
	void SetBehavior() noexcept
	{
		CurrentBehavior = Behaviors.at(typeid(T)).get();
	}
	
	void SetTarget(FVector2D const&) const;
	[[nodiscard]] FVector2D GetOwnerLocation2D() const noexcept;
	
protected:
	virtual void BeginPlay() override;

private:
	std::unordered_map<std::type_index, std::unique_ptr<FSteeringBehaviorBase_StukalovsAlex>> Behaviors{};
	FSteeringBehaviorBase_StukalovsAlex* CurrentBehavior{};// non-owning

	UPROPERTY(VisibleAnywhere)
	float DegPerSec{ 90.f };

	ASurvivorPawn* SurvivorPawn{};
	FVector2D SmoothedVelocity{};	
};
