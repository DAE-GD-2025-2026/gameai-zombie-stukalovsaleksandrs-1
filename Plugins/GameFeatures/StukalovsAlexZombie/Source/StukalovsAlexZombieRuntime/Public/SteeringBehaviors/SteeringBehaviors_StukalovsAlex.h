#pragma once

#include "SteeringBehaviors/SteeringHelpers_StukalovsAlex.h"

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
    
};

class FSeek_StukalovsAlex : public FSteeringBehaviorBase_StukalovsAlex
{
public:
    explicit FSeek_StukalovsAlex(float DegPerSec = 90.f) noexcept;
    virtual ~FSeek_StukalovsAlex() override;
    virtual FSteeringOutput_StukalovsAlex CalculateSteering(float DeltaTime, USteeringComponent_StukalovsAlex const&) noexcept override;

private:
    float DegPerSec{};
    
};
