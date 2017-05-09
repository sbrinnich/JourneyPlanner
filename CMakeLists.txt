cmake_minimum_required(VERSION 3.7)
project(JourneyPlanner)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -static -static-libgcc -static-libstdc++")

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")

set(SOURCE_FILES main.cpp)
add_executable(JourneyPlanner ${SOURCE_FILES})