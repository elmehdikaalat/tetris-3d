#include "GameField.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

GameField::GameField() : gameState(GameState::WAITING_TO_START), currentPiece(nullptr), 
                         score(0), linesCleared(0), rng(std::time(0)), pieceDist(0, 5) {
    // Initialize field grid
    field.resize(FIELD_HEIGHT);
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        field[y].resize(FIELD_WIDTH, nullptr);
    }
    
    uiOverlay = new UIOverlay();
    initializeWalls();
    
    // Setup camera and projection
    view = glm::lookAt(
        glm::vec3(20.0f, 25.0f, 70.0f),
        glm::vec3(4.5f, 7.5f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    
    projection = glm::perspective(
        glm::radians(15.0f),
        1200.0f / 900.0f, 
        0.1f, 
        100.0f
    );
}

GameField::~GameField() {
    clearField();
    
    // Clean up walls
    for (Cube* wall : walls) {
        delete wall;
    }
    
    delete currentPiece;
    delete uiOverlay;
}

void GameField::clearField() {
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            delete field[y][x];
            field[y][x] = nullptr;
        }
    }
}

void GameField::initializeWalls() {
    glm::vec3 wallColor(0.3f, 0.3f, 0.3f);
    
    // Bottom wall
    for (int x = -1; x <= FIELD_WIDTH; x++) {
        walls.push_back(new Cube(x, -1, 0, wallColor));
    }
    
    // Left wall
    for (int y = 0; y < FIELD_HEIGHT + 2; y++) {
        walls.push_back(new Cube(-1, y, 0, wallColor));
    }
    
    // Right wall
    for (int y = 0; y < FIELD_HEIGHT + 2; y++) {
        walls.push_back(new Cube(FIELD_WIDTH, y, 0, wallColor));
    }
}

void GameField::startGame() {
    if (gameState != GameState::WAITING_TO_START) return;
    
    resetGame();
    gameState = GameState::PLAYING;
    spawnNewPiece();
}

void GameField::resetGame() {
    clearField();
    delete currentPiece;
    currentPiece = nullptr;
    score = 0;
    linesCleared = 0;
    gameState = GameState::WAITING_TO_START;
}

void GameField::spawnNewPiece() {
    if (gameState != GameState::PLAYING) return;
    
    PieceType type = static_cast<PieceType>(pieceDist(rng));
    currentPiece = new Piece(type, 5.0f, FIELD_HEIGHT);
    
    // Check if spawn position is valid
    if (!isValidPosition(currentPiece->getBlockPositions())) {
        gameState = GameState::GAME_OVER;
        std::cout << "Game Over! Score: " << score << " Lines: " << linesCleared << std::endl;
    }
}

bool GameField::isValidPosition(const std::vector<glm::vec2>& positions) {
    for (const auto& pos : positions) {
        int x = static_cast<int>(round(pos.x));
        int y = static_cast<int>(round(pos.y));
        
        if (x < 0 || x >= FIELD_WIDTH || y < 0) {
            return false;
        }
        
        if (y < FIELD_HEIGHT && field[y][x] != nullptr) {
            return false;
        }
    }
    return true;
}

void GameField::moveCurrentPiece(int dx, int dy) {
    if (!currentPiece || gameState != GameState::PLAYING) return;
    
    currentPiece->move(dx, dy);
    
    if (!isValidPosition(currentPiece->getBlockPositions())) {
        currentPiece->move(-dx, -dy);
    }
}

void GameField::dropCurrentPiece() {
    if (!currentPiece || gameState != GameState::PLAYING) return;
    
    while (true) {
        currentPiece->move(0, -1);
        if (!isValidPosition(currentPiece->getBlockPositions())) {
            currentPiece->move(0, 1);
            break;
        }
    }
    
    lockCurrentPiece();
}

void GameField::lockCurrentPiece() {
    if (!currentPiece) return;
    
    std::vector<glm::vec2> positions = currentPiece->getBlockPositions();
    
    // Check if any part exceeds field height - GAME OVER
    for (const auto& pos : positions) {
        int y = static_cast<int>(round(pos.y));
        if (y >= FIELD_HEIGHT) {
            gameState = GameState::GAME_OVER;
            delete currentPiece;
            currentPiece = nullptr;
            std::cout << "Game Over! Score: " << score << " Lines: " << linesCleared << std::endl;
            return;
        }
    }
    
    // Place cubes in field
    glm::vec3 pieceColor = currentPiece->getColor();
    for (const auto& pos : positions) {
        int x = static_cast<int>(round(pos.x));
        int y = static_cast<int>(round(pos.y));
        field[y][x] = new Cube(x, y, 0, pieceColor);
    }
    
    delete currentPiece;
    currentPiece = nullptr;
    
    checkAndClearLines();
    spawnNewPiece();
}

void GameField::update() {
    if (!currentPiece || gameState != GameState::PLAYING) return;
    
    currentPiece->move(0, -1);
    
    if (!isValidPosition(currentPiece->getBlockPositions())) {
        currentPiece->move(0, 1);
        lockCurrentPiece();
    }
}

void GameField::checkAndClearLines() {
    int clearedThisTurn = 0;
    
    for (int y = FIELD_HEIGHT - 1; y >= 0; y--) {
        if (isLineFull(y)) {
            clearLine(y);
            dropLinesAbove(y);
            clearedThisTurn++;
            y++; // Check this line again
        }
    }
    
    if (clearedThisTurn > 0) {
        linesCleared += clearedThisTurn;
        score += clearedThisTurn * 100 * (clearedThisTurn > 1 ? 2 : 1); // Bonus for multiple lines
        std::cout << "Lines cleared: " << clearedThisTurn << " Total: " << linesCleared << " Score: " << score << std::endl;
    }
}

bool GameField::isLineFull(int line) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
        if (field[line][x] == nullptr) {
            return false;
        }
    }
    return true;
}

void GameField::clearLine(int line) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
        delete field[line][x];
        field[line][x] = nullptr;
    }
}

void GameField::dropLinesAbove(int clearedLine) {
    for (int y = clearedLine + 1; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (field[y][x] != nullptr) {
                field[y-1][x] = field[y][x];
                field[y-1][x]->setPosition(x, y-1, 0);
                field[y][x] = nullptr;
            }
        }
    }
}

void GameField::renderStartScreen() {
    // Render walls
    for (Cube* wall : walls) {
        wall->render(view, projection);
    }
    
    // Render UI messages
    uiOverlay->renderStartMessage(view, projection);
    uiOverlay->renderControls(view, projection);
}

void GameField::renderGameOverScreen() {
    // Render walls and placed pieces
    for (Cube* wall : walls) {
        wall->render(view, projection);
    }
    
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (field[y][x] != nullptr) {
                field[y][x]->render(view, projection);
            }
        }
    }
    
    // Render game over UI
    uiOverlay->renderGameOverMessage(view, projection, score, linesCleared);
}

void GameField::render() {
    switch (gameState) {
        case GameState::WAITING_TO_START:
            renderStartScreen();
            break;
            
        case GameState::PLAYING:
            // Render walls
            for (Cube* wall : walls) {
                wall->render(view, projection);
            }
            
            // Render placed pieces
            for (int y = 0; y < FIELD_HEIGHT; y++) {
                for (int x = 0; x < FIELD_WIDTH; x++) {
                    if (field[y][x] != nullptr) {
                        field[y][x]->render(view, projection);
                    }
                }
            }
            
            // Render current falling piece
            if (currentPiece) {
                currentPiece->render(view, projection);
            }
            break;
            
        case GameState::GAME_OVER:
            renderGameOverScreen();
            break;
    }
}