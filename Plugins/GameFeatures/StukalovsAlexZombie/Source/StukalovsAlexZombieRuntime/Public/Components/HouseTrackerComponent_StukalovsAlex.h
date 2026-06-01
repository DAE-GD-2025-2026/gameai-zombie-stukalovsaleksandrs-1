// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HouseTrackerComponent_StukalovsAlex.generated.h"

class AHouse;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STUKALOVSALEXZOMBIERUNTIME_API UHouseTrackerComponent_StukalovsAlex : public UActorComponent
{
	GENERATED_BODY()

public:
	UHouseTrackerComponent_StukalovsAlex();
	
	[[nodiscard]] bool IsHouseVisited(AHouse const&) const noexcept;
	void SaveHouseAsVisited(AHouse&) noexcept;
	void MarkHouseOwnerIsInAsVisited() noexcept;

protected:
	virtual void BeginPlay() override;
	
private:
	TArray<AHouse*> Houses;
	TArray<AHouse*> VisitedHouses{};
	static uint32_t constexpr MaxVisitedHouses{5u};
	uint32_t OldestHouseIdx{};// Index of the house to get overriden next

	void GetAllHouses() noexcept;
	[[nodiscard]] AHouse* GetHouseOwnerIsIn() noexcept;
};
