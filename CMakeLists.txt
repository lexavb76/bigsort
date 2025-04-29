cmake_minimum_required(VERSION 3.20)

project(bigsort LANGUAGES CXX)

set(INCLUDE_DIRS "include/" "include/sorters/")

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
