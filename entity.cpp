#include "entity.hpp"

namespace iron_dome_game
{

Pos Entity::pos() const
{
    if (isStatic())
        return trajectory.initialState.pos;
    return trajectory.calculatePosition();
}

BoundingBox Entity::boundingBox() const
{
    Pos p = pos();
    BoundingBox bbox;
    bbox.p1 = p;
    bbox.p2.x = p.x + width  - 1;
    bbox.p2.y = p.y + height - 1;
    return bbox;
}

}
