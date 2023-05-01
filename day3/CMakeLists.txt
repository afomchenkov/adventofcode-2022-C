# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# project name and language
project(day3 LANGUAGES CXX)

add_executable(day3 day3.cpp)
