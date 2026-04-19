#pragma once

// Grid
#define GRID_ROWS               (40)
#define GRID_COLUMNS            (120)

// Physics
#define GRAVITY                 (-10)   // pixels / sec^2

// Game loop
#define GAME_RUN_TIME_SEC       (60)
#define RENDER_INTERVAL_MS      (100)
#define GAME_TICK_MS            (10)

// Cannon / rocket
#define CANNON_X                (5)
#define CANNON_Y                (0)
#define CANNON_WIDTH            (3)
#define CANNON_HEIGHT           (2)
#define ROCKET_SPEED            (100.0f)
#define ROCKET_MAX_LIFETIME     (10.0f)
#define ROCKET_DEFAULT_ANGLE    (60)    // degrees, fallback when no target visible

// Pitcher / plate
#define PITCHER_X_OFFSET        (7)     // columns from right edge
#define PITCHER_WIDTH           (6)
#define PITCHER_HEIGHT          (5)

#define PLATE_LAUNCH_ANGLE      (120)   // degrees
#define PLATE_MIN_FIRE_POWER    (30)
#define PLATE_FIRE_POWER_RANGE  (15)
#define PLATE_SPAWN_INTERVAL_SEC (2)
#define PLATE_SPAWN_X_OFFSET    (10)    // columns from right edge
#define PLATE_SPAWN_Y           (5)
#define PLATE_WIDTH             (3)
#define PLATE_HEIGHT            (3)

// Scoring
#define SCORE_BASE              (100)
#define SCORE_MAX_SPEED         (50.0f)
