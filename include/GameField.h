#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Piece.h"
#include "Cube.h"
#include <vector>
#include <glm/glm.hpp>
#include <random>

class GameField {
public:
    static const int FIELD_WIDTH = 10;
    static const int FIELD_HEIGHT = 15;  // Reduced from 20

    GameField();
    ~GameField();

    void render();
    void update();
    void moveCurrentPiece(int dx, int dy);
    void dropCurrentPiece();
    
    bool isGameOver() const { return gameOver; }

private:
    void initializeWalls();
    void spawnNewPiece();
    bool isValidPosition(const std::vector<glm::vec2>& positions);
    void lockCurrentPiece();
    void checkAndClearLines();
    bool isLineFull(int line);
    void clearLine(int line);
    void dropLinesAbove(int line);
    
    // Game state
    std::vector<std::vector<Cube*>> field; // [y][x] - placed cubes
    std::vector<Cube*> walls; // Border walls
    Piece* currentPiece;
    bool gameOver;
    
    // Camera and projection
    glm::mat4 view;
    glm::mat4 projection;
    
    // Random generator
    std::mt19937 rng;
    std::uniform_int_distribution<int> pieceDist;
};

#endif