#include <iostream>
#include <string>
#include <random>
#include <cstdlib>
#include <memory>
#include <cmath>

#include "game.hpp"
#include "cannon.hpp"
#include "plate.hpp"
#include "rocket.hpp"

#define DEG_TO_RAD(x)   (x * 0.0174533f)

namespace iron_dome_game
{

Game::Game()
{
    grid.addEntity(std::make_shared<Cannon>());
    grid.addEntity(std::make_shared<Pitcher>());
}

//============================================================================//

void Game::keyboardListener()
{
    std::string line;
    while (gameIsActive && std::getline(std::cin, line))
        isShotFired = true;
}

//============================================================================//

void Game::fireRocket()
{
    ++shotsFired;

    // Aim at the oldest plate in the entity list.
    std::shared_ptr<Plate> target;
    for (auto& e : grid.entities()) {
        if (auto plate = std::dynamic_pointer_cast<Plate>(e)) {
            target = plate;
            break;
        }
    }

    float angle;
    if (target) {
        // Quadratic intercept: gravity cancels in relative motion, reducing the problem to finding T where rocket reaches the plate.
        float elapsed = target->trajectory.elapsed();

        Pos   pPos    = target->pos();
        float pvx     = target->trajectory.initialState.velocity.x;
        float pvy_eff = target->trajectory.initialState.velocity.y + GRAVITY * elapsed;

        float A = static_cast<float>(pPos.x) - CANNON_X;
        float B = pvx;
        float C = static_cast<float>(pPos.y) - CANNON_Y;
        float D = pvy_eff;
        float S = ROCKET_SPEED;

        float a = S*S - B*B - D*D;
        float b = -2.0f * (A*B + C*D);
        float c = -(A*A + C*C);

        float T = -1.0f;
        if (std::abs(a) < 1e-6f) {
            if (std::abs(b) > 1e-6f) T = -c / b;
        } else {
            float disc = b*b - 4*a*c;
            if (disc >= 0) {
                float sqrtDisc = std::sqrt(disc);
                float T1 = (-b + sqrtDisc) / (2*a);
                float T2 = (-b - sqrtDisc) / (2*a);
                if (T1 > 0 && T2 > 0) T = std::min(T1, T2);
                else T = std::max(T1, T2);
            }
        }

        if (T > 0) {
            float rvx = A/T + B;
            float rvy = C/T + D;
            angle = std::atan2(rvy, rvx);
        } else {
            // No valid solution: aim at the plate's current position as fallback.
            angle = std::atan2(C, A);
        }
    } else {
        angle = DEG_TO_RAD(ROCKET_DEFAULT_ANGLE);
    }

    grid.addEntity(std::make_shared<Rocket>(angle, ROCKET_SPEED));
}

//============================================================================//

void Game::play()
{
    gameIsActive = true;

    std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
    std::thread keyboardThread(&Game::keyboardListener, this);

    std::chrono::steady_clock::time_point lastTimeRefreshed = std::chrono::steady_clock::now();

    while (gameIsActive)
    {
        if (isShotFired)
        {
            isShotFired = false;
            fireRocket();
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - lastTimeRefreshed).count() > RENDER_INTERVAL_MS)
        {
            system("clear");
            grid.refresh();
            grid.draw();
            lastTimeRefreshed = std::chrono::steady_clock::now();
        }

        auto result = grid.checkHits();
        platesHit += static_cast<uint16_t>(result.hits.size());

        for (auto& hit : result.hits) {
            auto plate  = std::dynamic_pointer_cast<Plate>(hit.plate);
            auto rocket = std::dynamic_pointer_cast<Rocket>(hit.rocket);
            if (plate && rocket) {
                int score = scoreCalculator.calculateScore(plate, rocket, hit.hitTime);
                statistics.recordHit(score, hit.hitTime);
            }
        }
        for (uint16_t i = 0; i < result.missedRockets; ++i)
            statistics.recordMiss();

        std::this_thread::sleep_for(std::chrono::milliseconds(GAME_TICK_MS));

        auto gameTime = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - t0).count();
        if (gameTime > GAME_RUN_TIME_SEC)
            gameIsActive = false;
        else if (gameTime / PLATE_SPAWN_INTERVAL_SEC > platesFired) {
            spawnPlate();
            ++platesFired;
        }
    }

    std::cout << "\n=== Game Over ===\n";
    std::cout << statistics.getFormattedStats() << "\n";
    keyboardThread.detach();
}

//============================================================================//

void Game::spawnPlate()
{
    int firePower = std::rand() % PLATE_FIRE_POWER_RANGE + PLATE_MIN_FIRE_POWER;
    Velocity velocity;
    velocity.x = static_cast<int16_t>(std::cos(DEG_TO_RAD(PLATE_LAUNCH_ANGLE)) * firePower);
    velocity.y = static_cast<int16_t>(std::sin(DEG_TO_RAD(PLATE_LAUNCH_ANGLE)) * firePower);
    grid.addEntity(std::make_shared<Plate>(velocity));
}

}
