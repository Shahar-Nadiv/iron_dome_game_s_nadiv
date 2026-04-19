cmake_minimum_required(VERSION 3.20)
project(iron_dome_game)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(iron_dome_game
        main.cpp
        entity.cpp
        cannon.cpp
        game.cpp
        game_statistics.cpp
        grid.cpp
        pitcher.cpp
        plate.cpp
        rocket.cpp
        score_calculator.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(iron_dome_game Threads::Threads)
