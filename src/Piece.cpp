#include "Piece.h"
#include <random>
#include <ctime>

Piece::Piece(PieceType type, float x, float y) : type(type), x(x), y(y) {
    color = getRandomColor();
    initializePiece(type);
    updateCubePositions();
}

Piece::~Piece() {
    // nettoie les cubes
    for (Cube* cube : cubes) {
        delete cube;
    }
}

void Piece::initializePiece(PieceType type) {
    shape.clear();
    
    // formes des pieces tetris classiques
    switch (type) {
        case PieceType::I: // barre droite
            shape = {{-2, 0}, {-1, 0}, {0, 0}, {1, 0}};
            break;
        case PieceType::T: // piece en T
            shape = {{0, 0}, {-1, 0}, {1, 0}, {0, 1}};
            break;
        case PieceType::S: // piece en S
            shape = {{0, 0}, {0, 1}, {1, 1}, {1, 2}};
            break;
        case PieceType::Z: // piece en Z
            shape = {{1, 0}, {1, 1}, {0, 1}, {0, 2}};
            break;
        case PieceType::J: // piece en J
            shape = {{0, 0}, {0, 1}, {0, -1}, {-1, -1}};
            break;
        case PieceType::L: // piece en L
            shape = {{0, 0}, {0, 1}, {0, -1}, {1, -1}};
            break;
        default: // fallback sur la barre
            shape = {{-2, 0}, {-1, 0}, {0, 0}, {1, 0}};
            break;
    }
    
    // cree un cube pour chaque bloc de la piece
    for (size_t i = 0; i < shape.size(); i++) {
        cubes.push_back(new Cube(0, 0, 0, color));
    }
}

void Piece::updateCubePositions() {
    // met a jour la position de chaque cube
    for (size_t i = 0; i < cubes.size(); i++) {
        cubes[i]->setPosition(x + shape[i].x, y + shape[i].y, 0);
        cubes[i]->setColor(color);
    }
}

void Piece::render(const glm::mat4& view, const glm::mat4& projection) {
    // dessine tous les cubes de la piece
    for (Cube* cube : cubes) {
        cube->render(view, projection);
    }
}

void Piece::move(float dx, float dy) {
    // deplace la piece
    x += dx;
    y += dy;
    updateCubePositions();
}

void Piece::setPosition(float x, float y) {
    // change la position de la piece
    this->x = x;
    this->y = y;
    updateCubePositions();
}

std::vector<glm::vec2> Piece::getBlockPositions() const {
    // retourne toutes les positions des blocs
    std::vector<glm::vec2> positions;
    for (const auto& block : shape) {
        positions.push_back({x + block.x, y + block.y});
    }
    return positions;
}

glm::vec3 Piece::getRandomColor() {
    static std::mt19937 rng(time(0));
    static std::uniform_real_distribution<float> dist(0.2f, 0.9f);
    
    // genere des couleurs vives
    float r = dist(rng);
    float g = dist(rng);
    float b = dist(rng);
    
    // assure qu'au moins une composante soit claire
    float maxComp = std::max({r, g, b});
    if (maxComp < 0.7f) {
        if (r == maxComp) r = 0.8f;
        else if (g == maxComp) g = 0.8f;
        else b = 0.8f;
    }
    
    return glm::vec3(r, g, b);
}