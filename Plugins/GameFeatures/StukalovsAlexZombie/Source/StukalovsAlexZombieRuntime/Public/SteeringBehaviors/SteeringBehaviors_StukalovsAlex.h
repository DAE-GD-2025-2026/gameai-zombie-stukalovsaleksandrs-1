#pragma once

#include "SteeringBehaviors/SteeringOutput_StukalovsAlex.h"
class USteeringComponent_StukalovsAlex;

class FSteeringBehaviorBase_StukalovsAlex
{
public:
    FSteeringBehaviorBase_StukalovsAlex() noexcept = default;
    virtual ~FSteeringBehaviorBase_StukalovsAlex() noexcept = default;

    virtual FSteeringOutput_StukalovsAlex CalculateSteering(float DeltaTime, USteeringComponent_StukalovsAlex const&) noexcept = 0;

    void SetTarget(FVector2D const& NewTarget) noexcept { Target = NewTarget; }
    [[nodiscard]] FVector2D GetTarget() const noexcept { return Target; }
    
    template<class T, std::enable_if_t<std::is_base_of_v<FSteeringBehaviorBase_StukalovsAlex, T>>* = nullptr>
    [[nodiscard]] T* As() noexcept { return static_cast<T*>(this); }

protected:
    FVector2D Target;

    [[nodiscard]] static FVector2D GetClosestNavigablePoint(FVector2D Point, AActor& Owner); 
};

class FIdle_StukalovsAlex final : public FSteeringBehaviorBase_StukalovsAlex
{
public:
    FSteeringOutput_StukalovsAlex CalculateSteering(float DeltaTime, USteeringComponent_StukalovsAlex const&) noexcept override;
};

class FSeek_StukalovsAlex : public FSteeringBehaviorBase_StukalovsAlex
{
public:
    virtual FSteeringOutput_StukalovsAlex CalculateSteering(float DeltaTime, USteeringComponent_StukalovsAlex const&) noexcept override;
    virtual ~FSeek_StukalovsAlex() noexcept = default;
};

class FFlight_StukalovsAlex final : public FSeek_StukalovsAlex
{
public:
    virtual FSteeringOutput_StukalovsAlex CalculateSteering(float DeltaTime, USteeringComponent_StukalovsAlex const&) noexcept override;
};

class FLookAt_StukalovsAlex final : public FSteeringBehaviorBase_StukalovsAlex
{
public:
    virtual FSteeringOutput_StukalovsAlex CalculateSteering(float DeltaTime, USteeringComponent_StukalovsAlex const&) noexcept override;
};

class FWander_StukalovsAlex final : public FSeek_StukalovsAlex
{
public:
    virtual FSteeringOutput_StukalovsAlex CalculateSteering(float DeltaTime, USteeringComponent_StukalovsAlex const&) noexcept override;

private:
    // Radius of a circle for selecting random target points,
    // where center is the agent's center + agent's front vector times the offset.
    float const TargetCircleRadius{ 200 },
        TargetCircleOffset{ 400 }, 
        // Max offset between 2 consecutive random angles
        // NOTE: Added for smoothness
        MaxTargetDegreesOffset{ 1.f };
    float LastTargetDegrees{};
};

