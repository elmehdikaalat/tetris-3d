#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Piece.h"
#include "Cube.h"
#include "UIOverlay.h"
#include <vector>
#include <glm/glm.hpp>
#include <random>

enum class GameState {
    WAITING_TO_START,
    PLAYING,
    GAME_OVER
};

class GameField {
public:
    static const int FIELD_WIDTH = 10;
    static const int FIELD_HEIGHT = 15;

    GameField();
    ~GameField();

    void render();
    void update();
    void moveCurrentPiece(int dx, int dy);
    void dropCurrentPiece();
    void startGame();
    void resetGame();
    
    bool isGameOver() const { return gameState == GameState::GAME_OVER; }
    GameState getGameState() const { return gameState; }

private:
    void initializeWalls();
    void spawnNewPiece();
    bool isValidPosition(const std::vector<glm::vec2>& positions);
    void lockCurrentPiece();
    void checkAndClearLines();
    bool isLineFull(int line);
    void clearLine(int line);
    void dropLinesAbove(int line);
    void renderStartScreen();
    void renderGameOverScreen();
    void clearField();
    
    // Game state
    std::vector<std::vector<Cube*>> field;
    std::vector<Cube*> walls;
    Piece* currentPiece;
    GameState gameState;
    int score;
    int linesCleared;
    UIOverlay* uiOverlay;
    
    // Camera and projection
    glm::mat4 view;
    glm::mat4 projection;
    
    // Random generator
    std::mt19937 rng;
    std::uniform_int_distribution<int> pieceDist;
};

#endif