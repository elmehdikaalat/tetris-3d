#include "GameField.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

GameField::GameField() : gameState(GameState::PLAYING), currentPiece(nullptr), 
                         score(0), linesCleared(0), rng(static_cast<unsigned int>(std::time(0))), pieceDist(0, 5) {
    // Initialize field grid
    field.resize(FIELD_HEIGHT);
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        field[y].resize(FIELD_WIDTH, nullptr);
    }
    
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
    
    // Auto-start game
    spawnNewPiece();
    std::cout << "\n=== TETRIS 3D - GAME STARTED ===" << std::endl;
}

GameField::~GameField() {
    clearField();
    
    // Clean up walls
    for (Cube* wall : walls) {
        delete wall;
    }
    
    // Clean up indicators
    for (Cube* cube : indicatorCubes) {
        delete cube;
    }
    
    delete currentPiece;
}

void GameField::render() {
    // Render walls
    for (Cube* wall : walls) {
        wall->render(view, projection);
    }
    
    // Render field cubes
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (field[y][x] != nullptr) {
                field[y][x]->render(view, projection);
            }
        }
    }
    
    // Render current piece
    if (currentPiece != nullptr) {
        currentPiece->render(view, projection);
    }
    
    // Render game state indicators
    if (gameState == GameState::WAITING_TO_START) {
        renderStartIndicator();
    } else if (gameState == GameState::GAME_OVER) {
        renderGameOverIndicator();
    }
}

void GameField::renderStartIndicator() {
    // Clear previous indicators
    for (Cube* cube : indicatorCubes) {
        delete cube;
    }
    indicatorCubes.clear();
    
    // Create green indicator cubes spelling "START"
    glm::vec3 green(0.0f, 1.0f, 0.0f);
    
    // Simple pattern in the center of the field
    for (int x = 2; x <= 7; x++) {
        indicatorCubes.push_back(new Cube(static_cast<float>(x), 7.0f, 0.0f, green));
    }
    for (int x = 3; x <= 6; x++) {
        indicatorCubes.push_back(new Cube(static_cast<float>(x), 8.0f, 0.0f, green));
    }
    
    // Render indicators
    for (Cube* cube : indicatorCubes) {
        cube->render(view, projection);
    }
}

void GameField::renderGameOverIndicator() {
    // Clear previous indicators
    for (Cube* cube : indicatorCubes) {
        delete cube;
    }
    indicatorCubes.clear();
    
    // Create red indicator cubes
    glm::vec3 red(1.0f, 0.0f, 0.0f);
    
    // Cross pattern
    for (int i = 0; i < 5; i++) {
        indicatorCubes.push_back(new Cube(static_cast<float>(2 + i), static_cast<float>(5 + i), 0.0f, red));
        indicatorCubes.push_back(new Cube(static_cast<float>(6 - i), static_cast<float>(5 + i), 0.0f, red));
    }
    
    // Render indicators
    for (Cube* cube : indicatorCubes) {
        cube->render(view, projection);
    }
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
        walls.push_back(new Cube(static_cast<float>(x), -1.0f, 0.0f, wallColor));
    }
    
    // Left wall
    for (int y = 0; y < FIELD_HEIGHT + 2; y++) {
        walls.push_back(new Cube(-1.0f, static_cast<float>(y), 0.0f, wallColor));
    }
    
    // Right wall
    for (int y = 0; y < FIELD_HEIGHT + 2; y++) {
        walls.push_back(new Cube(static_cast<float>(FIELD_WIDTH), static_cast<float>(y), 0.0f, wallColor));
    }
}

void GameField::startGame() {
    if (gameState != GameState::WAITING_TO_START) return;
    
    restartGame();
}

void GameField::restartGame() {
    clearField();
    delete currentPiece;
    currentPiece = nullptr;
    score = 0;
    linesCleared = 0;
    gameState = GameState::PLAYING;
    
    // Clear indicators
    for (Cube* cube : indicatorCubes) {
        delete cube;
    }
    indicatorCubes.clear();
    
    spawnNewPiece();
    std::cout << "\n=== GAME RESTARTED ===" << std::endl;
}

void GameField::spawnNewPiece() {
    if (gameState != GameState::PLAYING) return;
    
    PieceType type = static_cast<PieceType>(pieceDist(rng));
    currentPiece = new Piece(type, 5.0f, FIELD_HEIGHT);
    
    // Check if spawn position is valid
    if (!isValidPosition(currentPiece->getBlockPositions())) {
        gameState = GameState::GAME_OVER;
        std::cout << "\n=== GAME OVER ===" << std::endl;
        std::cout << "Final Score: " << score << std::endl;
        std::cout << "Lines Cleared: " << linesCleared << std::endl;
        std::cout << "Press any key to restart!" << std::endl;
    }
}

bool GameField::isValidPosition(const std::vector<glm::vec2>& positions) {
    for (const auto& pos : positions) {
        int x = static_cast<int>(pos.x);
        int y = static_cast<int>(pos.y);
        
        // Check bounds
        if (x < 0 || x >= FIELD_WIDTH || y < 0) {
            return false;
        }
        
        // Check collision with placed blocks
        if (y < FIELD_HEIGHT && field[y][x] != nullptr) {
            return false;
        }
    }
    return true;
}

void GameField::lockCurrentPiece() {
    if (!currentPiece) return;
    
    auto positions = currentPiece->getBlockPositions();
    auto color = currentPiece->getColor();
    
    for (const auto& pos : positions) {
        int x = static_cast<int>(pos.x);
        int y = static_cast<int>(pos.y);
        
        if (y >= 0 && y < FIELD_HEIGHT && x >= 0 && x < FIELD_WIDTH) {
            field[y][x] = new Cube(static_cast<float>(x), static_cast<float>(y), 0.0f, color);
        }
    }
    
    delete currentPiece;
    currentPiece = nullptr;
    
    checkAndClearLines();
}

void GameField::checkAndClearLines() {
    for (int y = FIELD_HEIGHT - 1; y >= 0; y--) {
        if (isLineFull(y)) {
            clearLine(y);
            dropLinesAbove(y);
            linesCleared++;
            score += 100;
            y++; // Check the same line again as blocks have dropped
            
            std::cout << "Line cleared! Score: " << score << " | Lines: " << linesCleared << std::endl;
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
    for (int y = clearedLine; y < FIELD_HEIGHT - 1; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            field[y][x] = field[y + 1][x];
            field[y + 1][x] = nullptr;
            
            // Update cube position if it exists
            if (field[y][x] != nullptr) {
                // This would require modifying Cube class to update position
                // For now, recreate the cube at new position
                auto color = field[y][x]->getColor();
                delete field[y][x];
                field[y][x] = new Cube(static_cast<float>(x), static_cast<float>(y), 0.0f, color);
            }
        }
    }
}

void GameField::update() {
    if (gameState != GameState::PLAYING || !currentPiece) return;
    
    // Try to move piece down by checking if it can move
    auto newPositions = currentPiece->getBlockPositions();
    for (auto& pos : newPositions) {
        pos.y -= 1;
    }
    
    if (isValidPosition(newPositions)) {
        currentPiece->move(0, -1);  // Move down
    } else {
        // Piece can't move down, lock it
        lockCurrentPiece();
        spawnNewPiece();
    }
}

void GameField::moveCurrentPiece(int dx, int dy) {
    if (gameState != GameState::PLAYING || !currentPiece) return;
    
    auto newPositions = currentPiece->getBlockPositions();
    for (auto& pos : newPositions) {
        pos.x += dx;
        pos.y += dy;
    }
    
    if (isValidPosition(newPositions)) {
        currentPiece->move(dx, dy);
    }
}

void GameField::dropCurrentPiece() {
    if (gameState != GameState::PLAYING || !currentPiece) return;
    
    // Drop piece until it can't move down anymore
    bool canMoveDown = true;
    while (canMoveDown) {
        auto newPositions = currentPiece->getBlockPositions();
        for (auto& pos : newPositions) {
            pos.y -= 1;
        }
        
        if (isValidPosition(newPositions)) {
            currentPiece->move(0, -1);
        } else {
            canMoveDown = false;
        }
    }
    
    // Lock the piece
    lockCurrentPiece();
    spawnNewPiece();
}