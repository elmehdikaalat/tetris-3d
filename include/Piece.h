#ifndef PIECE_H
#define PIECE_H

#include "Cube.h"
#include <vector>
#include <glm/glm.hpp>

enum class PieceType {
    I, O, T, S, Z, J, L
};

class Piece {
public:
    Piece(PieceType type, float x = 0, float y = 0);
    ~Piece();

    void render(const glm::mat4& view, const glm::mat4& projection);
    void move(float dx, float dy);
    void setPosition(float x, float y);
    
    std::vector<glm::vec2> getBlockPositions() const;
    glm::vec3 getColor() const { return color; }
    float getX() const { return x; }
    float getY() const { return y; }

    static glm::vec3 getRandomColor();

private:
    void initializePiece(PieceType type);
    void updateCubePositions();

    PieceType type;
    std::vector<Cube*> cubes;
    std::vector<glm::vec2> shape; // Relative positions of blocks
    glm::vec3 color;
    float x, y; // Position of the piece (center)
};

#endif