// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include <typeindex>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <unordered_map>

#include "SteeringBehaviors.h"
#include "SteeringComponent.generated.h"

class FSteeringBehaviorBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STUKALOVSALEXZOMBIERUNTIME_API USteeringComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USteeringComponent();

	virtual void TickComponent(float DeltaSec, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	
	template<typename T> requires std::derived_from<T, FSteeringBehaviorBase>
	void SetBehavior() const
	{
		CurrentBehavior = *Behaviors.at(typeid(T));
	}
	
	void SetTarget(FVector2D const&) const;
	[[nodiscard]] FVector2D GetOwnerLocation2D() const noexcept;
	
protected:
	virtual void BeginPlay() override;

private:
	std::unordered_map<std::type_index, std::unique_ptr<FSteeringBehaviorBase>> Behaviors{};
	FSteeringBehaviorBase* CurrentBehavior{};// non-owning

};
