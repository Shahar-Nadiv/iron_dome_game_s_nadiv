#include "score_calculator.hpp"
#include "plate.hpp"
#include "rocket.hpp"
#include "config.hpp"
#include <cmath>
#include <algorithm>

namespace iron_dome_game
{

int DistanceBasedStrategy::calculateScore(
    std::shared_ptr<Plate> plate,
    std::shared_ptr<Rocket> rocket,
    float hitTime) const
{
    const float maxDistance = static_cast<float>(GRID_COLUMNS);

    Pos pos = plate->pos();
    float distance = std::sqrt(static_cast<float>(pos.x * pos.x + pos.y * pos.y));
    return static_cast<int>(SCORE_BASE * (distance / maxDistance));
}

int SpeedBasedStrategy::calculateScore(
    std::shared_ptr<Plate> plate,
    std::shared_ptr<Rocket> rocket,
    float hitTime) const
{
    const auto& vel = plate->trajectory.initialState.velocity;
    float speed = std::sqrt(static_cast<float>(vel.x * vel.x + vel.y * vel.y));
    return static_cast<int>(SCORE_BASE * std::min(speed / SCORE_MAX_SPEED, 1.0f));
}

int TimeBasedStrategy::calculateScore(
    std::shared_ptr<Plate> plate,
    std::shared_ptr<Rocket> rocket,
    float hitTime) const
{
    return static_cast<int>(SCORE_BASE * (1.0f / (hitTime + 1.0f)));
}

ScoreCalculator::ScoreCalculator()
    : m_strategy(std::make_shared<DistanceBasedStrategy>())
{}

void ScoreCalculator::setStrategy(std::shared_ptr<ScoringStrategy> strategy)
{
    m_strategy = strategy;
}

int ScoreCalculator::calculateScore(
    std::shared_ptr<Plate> plate,
    std::shared_ptr<Rocket> rocket,
    float hitTime) const
{
    if (!m_strategy) return 0;
    return m_strategy->calculateScore(plate, rocket, hitTime);
}

}
