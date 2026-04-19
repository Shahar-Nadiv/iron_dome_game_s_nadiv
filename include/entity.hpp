#pragma once

#include "config.hpp"
#include "grid.hpp"
#include "trajectory.hpp"

namespace iron_dome_game
{
enum EntityType
{
    NONE,
    PITCHER,
    CANNON,
    PLATE,
    ROCKET
};

struct Entity
{
    Trajectory trajectory;

    virtual EntityType type() const { return EntityType::NONE; }

    Pos pos() const;

    uint16_t width  = 0;
    uint16_t height = 0;

    BoundingBox boundingBox() const;

    virtual bool isStatic() const = 0;

    virtual void drawOnGrid(Grid& grid) = 0;

    virtual ~Entity() = default;
};
}
