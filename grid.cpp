#include <iostream>
#include <set>
#include <vector>
#include "grid.hpp"
#include "entity.hpp"
#include "rocket.hpp"

namespace iron_dome_game
{

void Grid::draw()
{

    for (int i = GRID_ROWS - 1; i >= 0; --i)
    {
        for (int j = 0; j < GRID_COLUMNS; ++j)
        {
            std::cout << m_grid[i][j];
        }
        std::cout << std::endl;
    }
}

//============================================================================//

void Grid::refresh() 
{
    // Draw background
    forEveryPixel(
        [this](int row, int col)
        {
            m_grid[row][col] = ' ';
        }
    );

    // Draw ground
    forEveryPixel(
        [this](int row, int col)
        {
            m_grid[row][col] = '_';
        },
        1
    );

    for (auto entity : m_entities)
    {
        entity->drawOnGrid(*this);
    }
}

//============================================================================//

bool Grid::drawPixel(uint16_t row, uint16_t col, char pixel) 
{
    if (row < rows() && col < columns() && pixel != ' ')
    {
        m_grid[row][col] = pixel;
        return true;
    }
    return false;
}

//============================================================================//

void Grid::forEveryPixel(std::function<void(int row, int col)> function, const int rowCount, const int columnCount) 
{
    for (int i = 0; i < rowCount; ++i)
    {
        for (int j = 0; j < columnCount; ++j)
        {
            function(i, j);
        }
    }
}

//============================================================================//

static bool isEntityOffScreen(const std::shared_ptr<Entity>& entity)
{
    if (entity->isStatic()) return false;
    const auto& traj = entity->trajectory;
    float t  = traj.elapsed();
    float px = traj.initialState.pos.x + traj.initialState.velocity.x * t;
    float py = traj.initialState.pos.y + traj.initialState.velocity.y * t
               + 0.5f * GRAVITY * t * t;
    return px < 0 || px >= GRID_COLUMNS || py < 0 || py >= GRID_ROWS;
}

CheckHitsResult Grid::checkHits()
{
    CheckHitsResult result;
    result.missedRockets = 0;

    std::vector<std::shared_ptr<Entity>> rockets, plates;
    for (auto& e : m_entities) {
        if (e->type() == EntityType::ROCKET)
            rockets.push_back(e);
        else if (e->type() == EntityType::PLATE)
            plates.push_back(e);
    }

    // Defer removal to avoid invalidating iterators during the loop.
    std::set<std::shared_ptr<Entity>> toRemove;

    for (auto& r : rockets) {
        auto rocket = std::dynamic_pointer_cast<Rocket>(r);
        if (rocket && rocket->isExpired()) {
            toRemove.insert(r);
            ++result.missedRockets;
        }
    }

    for (auto& p : plates) {
        if (isEntityOffScreen(p))
            toRemove.insert(p);
    }

    // Skip entities marked for removal so one rocket cannot register two hits per tick.
    for (auto& r : rockets) {
        if (toRemove.count(r)) continue;
        for (auto& p : plates) {
            if (toRemove.count(p)) continue;
            if (intersects(r, p)) {
                float hitTime = p->trajectory.elapsed();
                result.hits.push_back({p, r, hitTime});
                toRemove.insert(r);
                toRemove.insert(p);
                break;
            }
        }
    }

    m_entities.remove_if([&toRemove](const std::shared_ptr<Entity>& e) {
        return toRemove.count(e) > 0;
    });

    return result;
}

//============================================================================//

bool Grid::intersects(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second)
{
    BoundingBox bb1 = first->boundingBox();
    BoundingBox bb2 = second->boundingBox();

    // AABB overlap check using signed ints to avoid uint16_t wrap issues
    int x1lo = bb1.p1.x, x1hi = bb1.p2.x;
    int y1lo = bb1.p1.y, y1hi = bb1.p2.y;
    int x2lo = bb2.p1.x, x2hi = bb2.p2.x;
    int y2lo = bb2.p1.y, y2hi = bb2.p2.y;

    if (x1hi < x2lo || x2hi < x1lo) return false;
    if (y1hi < y2lo || y2hi < y1lo) return false;
    return true;
}

}