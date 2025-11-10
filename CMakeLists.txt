cmake_minimum_required(VERSION 3.10)
project(Tetris3D)

set(CMAKE_CXX_STANDARD 17)

# Inclure les dossiers
include_directories(include)
include_directories(libs/glm)


# Trouver les librairies nécessaires
find_package(OpenGL REQUIRED)

# Ajouter GLFW et GLAD (on les installera ensuite)
add_subdirectory(libs/glfw)
add_library(glad src/glad.c)

# Ajouter les fichiers sources
file(GLOB SRC_FILES src/*.cpp)
add_executable(Tetris3D ${SRC_FILES})

# Lier les dépendances
target_link_libraries(Tetris3D
    glfw
    glad
    ${OPENGL_LIBRARIES}
)
