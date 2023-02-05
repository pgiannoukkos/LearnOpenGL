cmake_minimum_required(VERSION 3.10)
project(LearnOpenGL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ASSIMP_BUILD_ZLIB ON)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

include_directories(vendor/glfw/include)
include_directories(vendor/glad/include)
include_directories(vendor/stb_image)
include_directories(vendor/glm)
include_directories(vendor/assimp/include)

file(GLOB SOURCES LearnOpenGL/src/*.h LearnOpenGL/src/*.cpp LearnOpenGL/src/*.c)
add_executable(LearnOpenGL ${SOURCES})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT LearnOpenGL)

add_subdirectory(vendor/glad)
target_link_libraries(LearnOpenGL glad)

add_subdirectory(vendor/glfw)
target_link_libraries(LearnOpenGL glfw)

add_subdirectory(vendor/stb_image)
target_link_libraries(LearnOpenGL stb_image)

add_subdirectory(vendor/assimp)
target_link_libraries(LearnOpenGL assimp)
