#pragma once
#include "BehaviorTree/BlackboardComponent.h"
#include "Village/House/House.h"

class UBehaviorTreeComponent;
class ASurvivorPawn;

namespace BTTUtils_StukalovsAlex
{
	[[nodiscard]] ASurvivorPawn* GetOwner(UBehaviorTreeComponent& OwnerComp) noexcept;

	template <typename T>
	[[nodiscard]] T* GetBlackboardObject(UBehaviorTreeComponent& OwnerComp, FName const& BlackboardKeyName) noexcept
	{
		UBlackboardComponent const * const BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
		verify(BlackboardComponent);
		
		T* const Object{ Cast<T>(BlackboardComponent->GetValueAsObject(BlackboardKeyName)) };
		
		return Object;
	}

	void SetSteeringTarget(ASurvivorPawn& SurvivorPawn, FVector2D NewTarget);

	[[nodiscard]] bool IsPointInHouse(FVector const& Point, FHouseBounds const& Bounds) noexcept;
}
