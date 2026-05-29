#pragma once

#include "SteeringBehaviors/SteeringHelpers.h"

class USteeringComponent;

class FSteeringBehaviorBase
{
public:
    FSteeringBehaviorBase() noexcept = default;
    virtual ~FSteeringBehaviorBase() noexcept = default;

    virtual FSteeringOutput CalculateSteering(float DeltaTime, USteeringComponent const&) noexcept = 0;

    void SetTarget(FVector2D const& NewTarget) noexcept { Target = NewTarget; }
    [[nodiscard]] FVector2D GetTarget() const noexcept { return Target; }
    
    template<class T, std::enable_if_t<std::is_base_of_v<FSteeringBehaviorBase, T>>* = nullptr>
    [[nodiscard]] T* As() noexcept { return static_cast<T*>(this); }

protected:
    FVector2D Target;
    
};

class FSeek : public FSteeringBehaviorBase
{
public:
    explicit FSeek(float DegPerSec = 90.f) noexcept;
    virtual ~FSeek() override;
    virtual FSteeringOutput CalculateSteering(float DeltaTime, USteeringComponent const&) noexcept override;

private:
    float DegPerSec{};
    
};
