#include "config.hpp"
#include "plate.hpp"

namespace iron_dome_game
{
Plate::Plate(Velocity velocity)
{
    trajectory.initialState.pos.x = GRID_COLUMNS - PLATE_SPAWN_X_OFFSET;
    trajectory.initialState.pos.y = PLATE_SPAWN_Y;
    trajectory.initialState.velocity.x = velocity.x;
    trajectory.initialState.velocity.y = velocity.y;

    width   = PLATE_WIDTH;
    height  = PLATE_HEIGHT;
}

//============================================================================//

void Plate::drawOnGrid(Grid &grid)
{
    auto col = pos().x;
    auto row = pos().y;

    grid.drawPixel(row, col,   '\\');
    grid.drawPixel(row, col+1, '_');
    grid.drawPixel(row, col+2, '/');

    grid.drawPixel(row+1, col  , '|');
    grid.drawPixel(row+1, col+1, ' ');
    grid.drawPixel(row+1, col+2, '|');

    grid.drawPixel(row+2, col  , '/');
    grid.drawPixel(row+2, col+1, '^');
    grid.drawPixel(row+2, col+2, '\\');
}

}