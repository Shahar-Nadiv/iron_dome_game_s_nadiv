#!/bin/bash

g++ grid.cpp entity.cpp cannon.cpp plate.cpp rocket.cpp game.cpp pitcher.cpp game_statistics.cpp score_calculator.cpp main.cpp -I$(pwd)/include -lpthread -std=c++20 -o iron_dome_game
