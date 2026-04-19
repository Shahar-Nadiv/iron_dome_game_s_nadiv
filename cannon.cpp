#include "cannon.hpp"
#include "config.hpp"

namespace iron_dome_game
{

Cannon::Cannon()
{
    trajectory.initialState.pos.x = CANNON_X;
    trajectory.initialState.pos.y = CANNON_Y;
    width  = CANNON_WIDTH;
    height = CANNON_HEIGHT;
}

void Cannon::drawOnGrid(Grid& grid)
{
    auto col = pos().x;
    auto row = pos().y;

    // Row 1: barrel
    grid.drawPixel(row + 1, col,     '[');
    grid.drawPixel(row + 1, col + 1, '=');
    grid.drawPixel(row + 1, col + 2, ']');

    // Row 0: base
    grid.drawPixel(row, col,     '|');
    grid.drawPixel(row, col + 1, '|');
    grid.drawPixel(row, col + 2, '|');
}

}
