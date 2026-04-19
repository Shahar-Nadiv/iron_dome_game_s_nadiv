#pragma once

#include "entity.hpp"

namespace iron_dome_game
{
struct Rocket : public Entity
{
    Rocket(float angle, float speed);
    ~Rocket() = default;

    EntityType type() const override { return EntityType::ROCKET; }

    void drawOnGrid(Grid& grid) override;

    bool isStatic() const override { return false; }

    bool isExpired() const;
};
}
