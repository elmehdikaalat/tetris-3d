#include "GameField.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

GameField::GameField() : gameOver(false), rng(std::time(0)), pieceDist(0, 5) {
    // Initialize field grid
    field.resize(FIELD_HEIGHT);
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        field[y].resize(FIELD_WIDTH, nullptr);
    }
    
    initializeWalls();
    
    // Setup camera and projection - front-facing angle, much further back
    view = glm::lookAt(
        glm::vec3(20.0f, 25.0f, 70.0f), // Very far back
        glm::vec3(4.5f, 7.5f, 0.0f),    // Look at center
        glm::vec3(0.0f, 1.0f, 0.0f)     // Up vector
    );
    
    projection = glm::perspective(
        glm::radians(15.0f),             // Very small FOV
        1200.0f / 900.0f, 
        0.1f, 
        100.0f
    );
    
    spawnNewPiece();
}

GameField::~GameField() {
    // Clean up field cubes
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            delete field[y][x];
        }
    }
    
    // Clean up walls
    for (Cube* wall : walls) {
        delete wall;
    }
    
    delete currentPiece;
}

void GameField::initializeWalls() {
    // Create gray walls around the playing field
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

void GameField::spawnNewPiece() {
    if (gameOver) return;
    
    PieceType type = static_cast<PieceType>(pieceDist(rng));
    // Spawn at integer position (center of 10-wide field)
    currentPiece = new Piece(type, 5.0f, FIELD_HEIGHT);
    
    // Check if spawn position is valid
    if (!isValidPosition(currentPiece->getBlockPositions())) {
        gameOver = true;
        std::cout << "Game Over!" << std::endl;
    }
}

bool GameField::isValidPosition(const std::vector<glm::vec2>& positions) {
    for (const auto& pos : positions) {
        int x = static_cast<int>(round(pos.x));
        int y = static_cast<int>(round(pos.y));
        
        // Check bounds (allow pieces to be above field initially)
        if (x < 0 || x >= FIELD_WIDTH || y < 0) {
            return false;
        }
        
        // Check collision with placed pieces only within field bounds
        if (y < FIELD_HEIGHT && field[y][x] != nullptr) {
            return false;
        }
    }
    return true;
}

void GameField::moveCurrentPiece(int dx, int dy) {
    if (!currentPiece || gameOver) return;
    
    // Try to move
    currentPiece->move(dx, dy);
    
    if (!isValidPosition(currentPiece->getBlockPositions())) {
        // Revert move
        currentPiece->move(-dx, -dy);
    }
}

void GameField::dropCurrentPiece() {
    if (!currentPiece || gameOver) return;
    
    // Drop until it can't drop anymore
    while (true) {
        currentPiece->move(0, -1);
        if (!isValidPosition(currentPiece->getBlockPositions())) {
            currentPiece->move(0, 1); // Revert last move
            break;
        }
    }
    
    lockCurrentPiece();
}

void GameField::lockCurrentPiece() {
    if (!currentPiece) return;
    
    // Place cubes in field
    std::vector<glm::vec2> positions = currentPiece->getBlockPositions();
    glm::vec3 pieceColor = currentPiece->getColor();
    
    for (const auto& pos : positions) {
        int x = static_cast<int>(round(pos.x));
        int y = static_cast<int>(round(pos.y));
        
        if (x >= 0 && x < FIELD_WIDTH && y >= 0 && y < FIELD_HEIGHT) {
            field[y][x] = new Cube(x, y, 0, pieceColor);
        }
    }
    
    delete currentPiece;
    currentPiece = nullptr;
    
    checkAndClearLines();
    spawnNewPiece();
}

void GameField::update() {
    if (!currentPiece || gameOver) return;
    
    // Try to move piece down
    currentPiece->move(0, -1);
    
    if (!isValidPosition(currentPiece->getBlockPositions())) {
        // Can't move down, revert and lock
        currentPiece->move(0, 1);
        lockCurrentPiece();
    }
}

void GameField::checkAndClearLines() {
    for (int y = FIELD_HEIGHT - 1; y >= 0; y--) {
        if (isLineFull(y)) {
            clearLine(y);
            dropLinesAbove(y);
            y++; // Check this line again after dropping
        }
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
                // Move cube down
                field[y-1][x] = field[y][x];
                field[y-1][x]->setPosition(x, y-1, 0);
                field[y][x] = nullptr;
            }
        }
    }
}

void GameField::render() {
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
    if (currentPiece && !gameOver) {
        currentPiece->render(view, projection);
    }
}