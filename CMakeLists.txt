cmake_minimum_required(VERSION 3.10)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)

# Glad
add_subdirectory(vendor/glad)

# Assimp
# set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_TESTS OFF)
# set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_BUILD_ZLIB ON)
add_subdirectory(vendor/assimp)

# spdlog
add_subdirectory(vendor/spdlog)

# stb_image
set(STB_DIR vendor/stb_image)
file(GLOB STB_SRC ${STB_DIR}/*.cpp)
add_library(stb_image ${STB_SRC})

# ImGui
set(IMGUI_DIR vendor/imgui)
file(GLOB IMGUI_SRC ${IMGUI_DIR}/*.cpp)
add_library(imgui ${IMGUI_SRC})
target_include_directories(imgui PRIVATE ${IMGUI_DIR})

# Main executable
file(GLOB SOURCES LearnOpenGL/src/*.cpp vendor/imgui/backends/imgui_impl_glfw.cpp vendor/imgui/backends/imgui_impl_opengl3.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE vendor/glfw/include vendor/glad/include vendor/assimp/include vendor/spdlog/include vendor/glm vendor/imgui vendor/stb_image)
target_link_libraries(${PROJECT_NAME} glfw glad assimp spdlog stb_image imgui)
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
