#include "game_statistics.hpp"
#include <sstream>
#include <iomanip>
#include <numeric>

namespace iron_dome_game
{

HitRecord::HitRecord(int score, float timeToHit)
    : timestamp(std::chrono::steady_clock::now()), score(score), timeToHit(timeToHit)
{}

GameStatistics::GameStatistics()
    : m_totalScore(0), m_totalHits(0), m_totalShots(0),
      m_currentStreak(0), m_bestStreak(0)
{}

void GameStatistics::recordHit(int score, float timeToHit)
{
    ++m_totalShots;
    ++m_totalHits;
    m_totalScore += score;
    m_hitHistory.emplace_back(score, timeToHit);
    updateStreak(true);
}

void GameStatistics::recordMiss()
{
    ++m_totalShots;
    updateStreak(false);
}

int GameStatistics::getTotalScore() const
{
    return m_totalScore;
}

uint16_t GameStatistics::getTotalHits() const
{
    return m_totalHits;
}

uint16_t GameStatistics::getTotalShots() const
{
    return m_totalShots;
}

float GameStatistics::getAccuracy() const
{
    if (m_totalShots == 0) return 0.0f;
    return static_cast<float>(m_totalHits) / m_totalShots * 100.0f;
}

float GameStatistics::getAverageTimeToHit() const
{
    if (m_hitHistory.empty()) return 0.0f;
    float total = std::accumulate(m_hitHistory.begin(), m_hitHistory.end(), 0.0f,
        [](float sum, const HitRecord& r) { return sum + r.timeToHit; });
    return total / static_cast<float>(m_hitHistory.size());
}

uint16_t GameStatistics::getBestStreak() const
{
    return m_bestStreak;
}

std::string GameStatistics::getFormattedStats() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "Score:          " << m_totalScore << "\n";
    oss << "Hits/Shots:     " << m_totalHits << " / " << m_totalShots << "\n";
    oss << "Accuracy:       " << getAccuracy() << "%\n";
    oss << "Avg time to hit:" << getAverageTimeToHit() << "s\n";
    oss << "Best streak:    " << m_bestStreak;
    return oss.str();
}

void GameStatistics::reset()
{
    m_totalScore    = 0;
    m_totalHits     = 0;
    m_totalShots    = 0;
    m_currentStreak = 0;
    m_bestStreak    = 0;
    m_hitHistory.clear();
}

void GameStatistics::updateStreak(bool isHit)
{
    if (isHit) {
        ++m_currentStreak;
        if (m_currentStreak > m_bestStreak)
            m_bestStreak = m_currentStreak;
    } else {
        m_currentStreak = 0;
    }
}

}
