cmake_minimum_required(VERSION 3.10)
project(stb_image)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# file(GLOB SOURCES src/*.h src/*.cpp src/*.c)
add_library(stb_image stb_image.cpp)

