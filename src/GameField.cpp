#include "GameField.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>

GameField::GameField() : currentPiece(nullptr), gameState(GameState::PLAYING),
                         score(0), linesCleared(0), rng(static_cast<unsigned int>(std::time(0))), pieceDist(0, 5) {
    // init le terrain vide
    field.resize(FIELD_HEIGHT);
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        field[y].resize(FIELD_WIDTH, nullptr);
    }
    
    initializeWalls();
    
    // camera qui regarde le terrain
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
    
    spawnNewPiece();
    std::cout << "\n=== TETRIS 3D - GAME STARTED ===" << std::endl;
}

GameField::~GameField() {
    clearField();
    
    // nettoie tout
    for (Cube* wall : walls) {
        delete wall;
    }
    
    for (Cube* cube : indicatorCubes) {
        delete cube;
    }
    
    delete currentPiece;
}

void GameField::render() {
    // dessine les murs
    for (Cube* wall : walls) {
        wall->render(view, projection);
    }
    
    // dessine les cubes poses
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (field[y][x] != nullptr) {
                field[y][x]->render(view, projection);
            }
        }
    }
    
    // dessine la piece qui tombe
    if (currentPiece != nullptr) {
        currentPiece->render(view, projection);
    }
}

void GameField::clearField() {
    // vide tout le terrain
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            delete field[y][x];
            field[y][x] = nullptr;
        }
    }
}

void GameField::initializeWalls() {
    glm::vec3 wallColor(0.3f, 0.3f, 0.3f);
    
    // mur du bas
    for (int x = -1; x <= FIELD_WIDTH; x++) {
        walls.push_back(new Cube(static_cast<float>(x), -1.0f, 0.0f, wallColor));
    }
    
    // mur de gauche
    for (int y = 0; y < FIELD_HEIGHT + 2; y++) {
        walls.push_back(new Cube(-1.0f, static_cast<float>(y), 0.0f, wallColor));
    }
    
    // mur de droite
    for (int y = 0; y < FIELD_HEIGHT + 2; y++) {
        walls.push_back(new Cube(static_cast<float>(FIELD_WIDTH), static_cast<float>(y), 0.0f, wallColor));
    }
}

void GameField::startGame() {
    if (gameState != GameState::WAITING_TO_START) return;
    restartGame();
}

void GameField::restartGame() {
    // remet tout a zero
    clearField();
    delete currentPiece;
    currentPiece = nullptr;
    score = 0;
    linesCleared = 0;
    gameState = GameState::PLAYING;
    
    for (Cube* cube : indicatorCubes) {
        delete cube;
    }
    indicatorCubes.clear();
    
    spawnNewPiece();
    std::cout << "\n=== GAME RESTARTED ===" << std::endl;
}

void GameField::spawnNewPiece() {
    if (gameState != GameState::PLAYING) return;
    
    // cree une piece aleatoire
    PieceType type = static_cast<PieceType>(pieceDist(rng));
    currentPiece = new Piece(type, 5.0f, FIELD_HEIGHT);
    
    // check si on peut la placer
    if (!isValidPosition(currentPiece->getBlockPositions())) {
        gameState = GameState::GAME_OVER;
        std::cout << "\n=== GAME OVER ===" << std::endl;
        std::cout << "Final Score: " << score << std::endl;
        std::cout << "Lines Cleared: " << linesCleared << std::endl;
        std::cout << "Press any key to restart!" << std::endl;
        return;
    }
}

bool GameField::isValidPosition(const std::vector<glm::vec2>& positions) {
    for (const auto& pos : positions) {
        int x = static_cast<int>(pos.x);
        int y = static_cast<int>(pos.y);
        
        // verifie les limites
        if (x < 0 || x >= FIELD_WIDTH || y < 0) {
            return false;
        }
        
        // verifie collision avec pieces deja posees
        if (y < FIELD_HEIGHT && field[y][x] != nullptr) {
            return false;
        }
    }
    return true;
}

void GameField::lockCurrentPiece() {
    if (!currentPiece) return;
    
    // pose la piece sur le terrain
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
    // cherche les lignes completes
    for (int y = FIELD_HEIGHT - 1; y >= 0; y--) {
        if (isLineFull(y)) {
            clearLine(y);
            dropLinesAbove(y);
            linesCleared++;
            score += 100;
            y++; // recheck la meme ligne
            
            std::cout << "Line cleared! Score: " << score << " | Lines: " << linesCleared << std::endl;
        }
    }
}

bool GameField::isLineFull(int line) {
    // check si la ligne est pleine
    for (int x = 0; x < FIELD_WIDTH; x++) {
        if (field[line][x] == nullptr) {
            return false;
        }
    }
    return true;
}

void GameField::clearLine(int line) {
    // supprime tous les cubes de la ligne
    for (int x = 0; x < FIELD_WIDTH; x++) {
        if (field[line][x] != nullptr) {
            delete field[line][x];
            field[line][x] = nullptr;
        }
    }
}

void GameField::dropLinesAbove(int clearedLine) {
    // fait descendre tout ce qui est au dessus
    for (int y = clearedLine; y < FIELD_HEIGHT - 1; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            field[y][x] = field[y + 1][x];
            field[y + 1][x] = nullptr;
            
            // update la position visuelle
            if (field[y][x] != nullptr) {
                field[y][x]->setPosition(static_cast<float>(x), static_cast<float>(y), 0.0f);
            }
        }
    }
}

void GameField::update() {
    if (gameState != GameState::PLAYING || !currentPiece) return;
    
    // fait tomber la piece automatiquement
    auto newPositions = currentPiece->getBlockPositions();
    for (auto& pos : newPositions) {
        pos.y -= 1;
    }
    
    if (isValidPosition(newPositions)) {
        currentPiece->move(0, -1);
    } else {
        // piece touchee, on la pose
        lockCurrentPiece();
        spawnNewPiece();
    }
}

void GameField::moveCurrentPiece(int dx, int dy) {
    if (gameState != GameState::PLAYING || !currentPiece) return;
    
    // essaie de bouger la piece
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
    
    // fait tomber d'un coup jusqu'en bas
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
    
    lockCurrentPiece();
    spawnNewPiece();
}