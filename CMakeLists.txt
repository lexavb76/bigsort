cmake_minimum_required(VERSION 3.20)

project(bigsort LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIRS "include/" "include/sorters/" "include/data_source_handlers/")
set(SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
