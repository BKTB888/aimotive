cmake_minimum_required(VERSION 3.31)
project(aimotive)

set(CMAKE_CXX_STANDARD 23)

add_executable(aimotive main.cpp
        Field.cpp
        Field.h
        Tile.cpp
        Tile.h)
