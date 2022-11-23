cmake_minimum_required(VERSION 3.10)
project(glad)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# file(GLOB SOURCES src/*.h src/*.cpp src/*.c)
add_library(glad src/glad.c)

