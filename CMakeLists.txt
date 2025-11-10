cmake_minimum_required(VERSION 3.10)
project(Tetris3D)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)
include_directories(libs/glm)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add GLFW from source
add_subdirectory(libs/glfw)

# Create GLAD library
add_library(glad src/glad.c)

# Add executable with all source files
file(GLOB SRC_FILES src/*.cpp)
add_executable(Tetris3D ${SRC_FILES})

# Link libraries
target_link_libraries(Tetris3D
    glfw
    glad
    ${OPENGL_LIBRARIES}
)