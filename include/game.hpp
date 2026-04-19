#pragma once

#include <thread>
#include <atomic>

#include "grid.hpp"
#include "pitcher.hpp"
#include "cannon.hpp"
#include "plate.hpp"
#include "rocket.hpp"
#include "score_calculator.hpp"
#include "game_statistics.hpp"

namespace iron_dome_game
{
class Game
{
public:
    Game();
    ~Game() = default;

    void play();

private:
    void keyboardListener();
    void fireRocket();
    void spawnPlate();

    Grid grid;

    std::atomic<bool> isShotFired{false};
    std::atomic<bool> gameIsActive{false};

    uint16_t platesFired = 0;
    uint16_t platesHit   = 0;
    uint16_t shotsFired  = 0;

    ScoreCalculator scoreCalculator;
    GameStatistics  statistics;
};
}
