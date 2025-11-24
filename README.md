# Tetris 3D - Projet OpenGL

## Description
Un jeu de tetris en 3D fait en C++ avec OpenGL, GLFW et GLM.

## Ce qu'il faut avoir
- CMake 3.10+
- Un compilateur C++17 
- Support OpenGL

## Libs incluses
Les librairies necessaires :
- **GLFW** - pour gerer la fenetre et le contexte OpenGL
- **GLM** - pour les maths OpenGL

Tout est pret a build directement

## Comment build

### Linux/Mac
```bash
mkdir build
cd build
cmake ..
make
```

## Controles
- **A/Fleche gauche**: Bouger a gauche
- **E/Fleche droite**: Bouger a droite  
- **S/Fleche bas**: Drop la piece direct
- **Escape**: Quitter
- **N'importe quelle touche**: Restart quand c'est game over


## Structure du projet
```
src/           # Fichiers source
include/       # Headers
build/         # Build output (pas inclus)
CMakeLists.txt # Config pour build
```