cmake_minimum_required(VERSION 3.29)
project(PD_final_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(PD_final_1
        backpack.cpp
        backpack.h
        character.cpp
        character.h
        clickgame.cpp
        clickgame.h
        dino_game.cpp
        dino_game.h
        item.cpp
        item.h
        maze.cpp
        maze.h
        redistributable.h
        redLight.cpp
        redLight.h)
