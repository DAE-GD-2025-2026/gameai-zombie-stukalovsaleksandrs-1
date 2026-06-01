// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HouseTrackerComponent_StukalovsAlex.h"

#include "Kismet/GameplayStatics.h"
#include "Village/House/House.h"


// Sets default values for this component's properties
UHouseTrackerComponent_StukalovsAlex::UHouseTrackerComponent_StukalovsAlex()
{
	PrimaryComponentTick.bCanEverTick = false;


	// Initializing VisitedHouses with nullptr
	VisitedHouses.Init(nullptr, MaxVisitedHouses);
}

void UHouseTrackerComponent_StukalovsAlex::BeginPlay()
{
	Super::BeginPlay();
	GetAllHouses();
}

bool UHouseTrackerComponent_StukalovsAlex::IsHouseVisited(AHouse const& House) const noexcept
{
	return VisitedHouses.Contains(&House);
}

void UHouseTrackerComponent_StukalovsAlex::SaveHouseAsVisited(AHouse& House) noexcept
{
	if (IsHouseVisited(House)) return;// Not re-saving a visited house
#ifdef ENABLE_DEBUG_VISUALIZATION
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Marked house as visited"));
#endif
	VisitedHouses[OldestHouseIdx] = &House;
	OldestHouseIdx = (OldestHouseIdx + 1) % MaxVisitedHouses;
}

void UHouseTrackerComponent_StukalovsAlex::MarkHouseOwnerIsInAsVisited() noexcept
{
	AHouse* const House{ GetHouseOwnerIsIn() };
	if (!House) return;
	SaveHouseAsVisited(*House);
	// Unsetting the blackboard variable
	
}

void UHouseTrackerComponent_StukalovsAlex::GetAllHouses() noexcept
{
	TArray<AActor*> HouseActors;
	UGameplayStatics::GetAllActorsOfClass(GetOwner()->GetWorld(), AHouse::StaticClass(), HouseActors);
	for (AActor* Actor : HouseActors)
	{
		if (AHouse* House = Cast<AHouse>(Actor))
		{
			Houses.Add(House);
		}
	}
}

AHouse* UHouseTrackerComponent_StukalovsAlex::GetHouseOwnerIsIn() noexcept
{
	if (!GetOwner()) return nullptr;
    
	FVector const OwnerLocation = GetOwner()->GetActorLocation();
    
	for (AHouse* House : Houses)
	{
		if (!IsValid(House)) continue;
        
		FVector Origin;
		FVector Extent;
		House->GetActorBounds(true, Origin, Extent);
        
		FBox const HouseBox(Origin - Extent, Origin + Extent);
        
		if (HouseBox.IsInsideXY(OwnerLocation))
		{
			return House;
		}
	}
    
	return nullptr;
}
