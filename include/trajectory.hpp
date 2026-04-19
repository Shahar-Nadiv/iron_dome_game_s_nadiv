#pragma once
#include <stdint.h>
#include <chrono>
#include <math.h>
#include "config.hpp"

namespace iron_dome_game
{
struct Pos
{
    uint16_t x;
    uint16_t y;
};

struct Velocity
{
    int16_t x;
    int16_t y;
};

struct InitialState
{
    Pos pos;
    Velocity velocity;
    std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
};

struct Trajectory
{
    InitialState initialState;

    float elapsed() const
    {
        return std::chrono::duration<float>(std::chrono::steady_clock::now() - initialState.t0).count();
    }

    // Standard kinematic equations: x = x0 + vx*t,  y = y0 + vy*t + 0.5*g*t^2
    Pos calculatePosition() const
    {
        float t = elapsed();
        Pos pos;
        pos.x = static_cast<uint16_t>(round(initialState.pos.x + initialState.velocity.x * t));
        pos.y = static_cast<uint16_t>(round(initialState.pos.y + initialState.velocity.y * t + 0.5 * GRAVITY * t * t));
        return pos;
    }
};
}
