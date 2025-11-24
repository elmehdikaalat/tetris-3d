cmake_minimum_required(VERSION 3.14)
project(Tetris3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)

# GLAD library
add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)

# Main executable
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# Link everything
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    glad
    ${CMAKE_DL_LIBS}
)

# Compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic
)