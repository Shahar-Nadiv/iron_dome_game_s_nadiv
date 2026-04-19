#pragma once

#include "entity.hpp"

namespace iron_dome_game
{
struct Cannon : public Entity
{
    Cannon();
    ~Cannon() = default;

    EntityType type() const override { return EntityType::CANNON; }

    void drawOnGrid(Grid& grid) override;

    bool isStatic() const override { return true; }
};
}
