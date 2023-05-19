cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

if(UNIX)
    # Linux-specific configuration
    message(STATUS "Linux Config")
    set(CMAKE_C_COMPILER "/usr/bin/clang")
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
elseif(WIN32)
    # Windows-specific configuration
    message(STATUS "Windows Config")
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_COMPILER "clang++")
else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

project(LearnOpenGL LANGUAGES C CXX)

if (NOT DEFINED CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 17)
    set(CMAKE_C_STANDARD_REQUIRED ON)
endif()

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/glfw)

# Glad
set(GLAD_DIR ${CMAKE_SOURCE_DIR}/vendor/glad)
file(GLOB GLAD_SRC ${GLAD_DIR}/src/*.c)
add_library(glad STATIC ${GLAD_SRC})
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)

# Assimp
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_NO_EXPORT ON)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_BUILD_GLTF_IMPORTER ON)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/assimp)

# spdlog
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/spdlog)

# stb_image
set(STB_DIR ${CMAKE_SOURCE_DIR}/vendor/stb_image)
file(GLOB STB_SRC ${STB_DIR}/*.cpp)
add_library(stb_image ${STB_SRC})

# ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/vendor/imgui)
file(GLOB IMGUI_SRC ${IMGUI_DIR}/*.cpp)
add_library(imgui ${IMGUI_SRC})
target_include_directories(imgui PRIVATE ${IMGUI_DIR})

# Main executable
file(GLOB_RECURSE SOURCES LearnOpenGL/src/*.cpp vendor/imgui/backends/imgui_impl_glfw.cpp vendor/imgui/backends/imgui_impl_opengl3.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE LearnOpenGL/src vendor/glfw/include vendor/glad/include vendor/assimp/include vendor/spdlog/include vendor/glm vendor/imgui vendor/stb_image)
target_link_libraries(${PROJECT_NAME} glfw glad assimp spdlog stb_image imgui)
target_compile_definitions(${PROJECT_NAME} PRIVATE "GL_DEBUG" "GLFW_INCLUDE_NONE" "_CRT_SECURE_NO_WARNINGS")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
