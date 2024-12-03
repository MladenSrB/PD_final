cmake_minimum_required(VERSION 3.29)
project(PD_final)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(PD_final
dino_game.cpp
character.cpp
item.cpp)
