cmake_minimum_required(VERSION 3.18)
project(untitled1)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled1_server serverMain.cpp)

add_executable(untitled1 main.cpp PlayingField.cpp
        PlayingField.h)

# Set a different output name for the second executable
set_target_properties(untitled1 PROPERTIES OUTPUT_NAME untitled1_game)