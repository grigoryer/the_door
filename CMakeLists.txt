cmake_minimum_required(VERSION 3.10.0)
project(chessEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")

add_executable(door_engine ${SOURCES})

target_include_directories(door_engine PRIVATE include)