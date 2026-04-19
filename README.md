# Iron Dome Game

A console based 2D Iron Dome simulation

## Overview

Plates (threats) are launched from the right side of the screen and arc leftward under gravity.
A cannon sits on the left and fires interceptor rockets when the player presses **Enter**.
The game runs for `GAME_RUN_TIME_SEC` seconds, then prints a statistics summary.


## Controls

| Key   | Action              |
|-------|---------------------|
| Enter | Fire interceptor    |

## Build & Run

**Visual Studio:** Open the project folder — VS code detects `CMakeLists.txt` automatically. Press the Run button.

**Manual (Linux):**
```bash
g++ grid.cpp entity.cpp cannon.cpp plate.cpp rocket.cpp game.cpp pitcher.cpp game_statistics.cpp score_calculator.cpp main.cpp -I$(pwd)/include -lpthread -std=c++20 -o iron_dome_game

```

**Build File:** Provided build file
```bash
./build.sh
```

## Configuration

All tunable parameters live in `include/config.hpp` — no need to search through source files.

| Parameter | Default | Description |
|---|---|---|
| `GRID_ROWS` / `GRID_COLUMNS` | 40 / 120 | Play area size |
| `GAME_RUN_TIME_SEC` | 60 | Game duration in seconds |
| `RENDER_INTERVAL_MS` | 100 | Screen refresh rate (ms) |
| `GAME_TICK_MS` | 10 | Main loop sleep (ms) |
| `GRAVITY` | -10 | Pixels / sec² (negative = downward) |
| `ROCKET_SPEED` | 100.0 | Interceptor speed (px/s) |
| `ROCKET_MAX_LIFETIME` | 10.0 | Rocket auto-expires after this many seconds |
| `ROCKET_DEFAULT_ANGLE` | 60° | Firing angle when no target exists |
| `PLATE_LAUNCH_ANGLE` | 120° | Direction plates are launched |
| `PLATE_MIN_FIRE_POWER` | 30 | Minimum plate speed |
| `PLATE_FIRE_POWER_RANGE` | 15 | Random speed added on top of minimum |
| `PLATE_SPAWN_INTERVAL_SEC` | 2 | Seconds between plate spawns |
| `SCORE_BASE` | 100 | Maximum points per hit |
| `SCORE_MAX_SPEED` | 50.0 | Speed used to normalise SpeedBasedStrategy |

## Architecture

### Class hierarchy

```
Entity (abstract)
├── Pitcher   — static threat launcher, right side, draws the launcher sprite
├── Plate     — moving target, follows projectile trajectory
└── Rocket    — interceptor, fired from cannon, expires off screen or after MAX_LIFETIME
```

`Grid` owns all entities via `std::shared_ptr` in a `std::list`. Each tick it:
1. Clears and redraws the character buffer (`refresh` + `draw`)
2. Runs collision detection and removes expired/hit entities (`checkHits`)

`Game` owns `Grid`, `GameStatistics`, and `ScoreCalculator`. It spawns a keyboard listener thread and runs the main game loop.

### Design decisions

**Coordinate system:** Row 0 is the ground (bottom of the display). `Grid::draw()` iterates from `GRID_ROWS-1` down to `0` so higher row indices appear higher on screen — matching standard mathematical y-up convention.

**Predictive aiming:** `fireRocket()` solves a quadratic equation for the earliest intercept time *T*, accounting for the plate's current velocity and gravity. Because gravity acts equally on both objects, their relative motion is linear, simplifying the problem. If no real solution exists the rocket is aimed at the plate's current position.

**Non-blocking keyboard input:** `getchar()` would block the keyboard thread after `gameIsActive` goes false, causing `join()` in the main thread to hang. Instead the listener polls

**Thread safety:** `isShotFired` and `gameIsActive` are `std::atomic<bool>` to avoid data races between the keyboard thread and the main game loop.

**Scoring strategies (Strategy pattern):** `ScoreCalculator` holds a `ScoringStrategy` pointer. Three strategies are provided — distance-based, speed-based, and time-based. Swapping strategy requires only `scoreCalculator.setStrategy(...)` — no changes to `Game` or `Grid`.

**Entity removal:** Expired/hit entities are collected into a `std::set` during `checkHits` and removed in a single `remove_if` pass afterward. This avoids iterator invalidation and ensures a rocket cannot register two hits in the same tick.

### Assumptions

- The terminal supports at least 120 columns and 40 rows. Smaller terminals will wrap.
- `PLATE_LAUNCH_ANGLE = 120°` launches plates to the left at an upward trajectory that arcs down under gravity — this is the "threat" arc.
- Plates that fly off any edge of the grid are silently removed (treated as escaped, not intercepted).
- A missed rocket (one that expires or leaves the grid without hitting anything) counts as one miss in the statistics.

## Statistics (end of game summary)

| Metric | Description |
|---|---|
| Score | Total accumulated score across all hits |
| Hits / Shots | Successful intercepts vs total rockets fired |
| Accuracy | `hits / shots × 100%` |
| Avg time to hit | Mean seconds from plate spawn to intercept |
| Best streak | Longest consecutive-hit run |
