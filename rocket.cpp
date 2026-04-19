#include "rocket.hpp"
#include "config.hpp"
#include <cmath>

namespace iron_dome_game
{

Rocket::Rocket(float angle, float speed)
{
    trajectory.initialState.pos.x = CANNON_X;
    trajectory.initialState.pos.y = CANNON_Y;
    trajectory.initialState.velocity.x = static_cast<int16_t>(std::cos(angle) * speed);
    trajectory.initialState.velocity.y = static_cast<int16_t>(std::sin(angle) * speed);
    width  = 1;
    height = 1;
}

void Rocket::drawOnGrid(Grid& grid)
{
    Pos p = pos();
    grid.drawPixel(p.y, p.x, '*');
}

bool Rocket::isExpired() const
{
    float elapsed = trajectory.elapsed();

    if (elapsed > ROCKET_MAX_LIFETIME)
        return true;

    float px = trajectory.initialState.pos.x + trajectory.initialState.velocity.x * elapsed;
    float py = trajectory.initialState.pos.y + trajectory.initialState.velocity.y * elapsed
               + 0.5f * GRAVITY * elapsed * elapsed;

    return px < 0 || px >= GRID_COLUMNS || py < 0 || py >= GRID_ROWS;
}

} // namespace iron_dome_game
