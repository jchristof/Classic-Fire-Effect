cmake_minimum_required(VERSION 3.13)
project(classic_fire_effect)

set(CMAKE_CXX_STANDARD 14)

add_executable(classic_fire_effect main.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(classic_fire_effect ${SDL2_LIBRARIES})