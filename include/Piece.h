#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Cube.h"

enum PieceType { I, L, J, O, T, S, Z };

class Piece {
public:
    glm::vec3 position;
    std::vector<Cube> cubes;
    float r, g, b;

    Piece(PieceType type, glm::vec3 pos = {0.0f, 10.5f, 0.0f}) : position(pos) {
        // Couleur aléatoire
        r = 0.3f + static_cast<float>(rand()) / RAND_MAX * 0.7f;
        g = 0.3f + static_cast<float>(rand()) / RAND_MAX * 0.7f;
        b = 0.3f + static_cast<float>(rand()) / RAND_MAX * 0.7f;

        switch (type) {
        case I:
            cubes = {
                Cube(pos.x - 1, pos.y, pos.z, r, g, b),
                Cube(pos.x, pos.y, pos.z, r, g, b),
                Cube(pos.x + 1, pos.y, pos.z, r, g, b),
                Cube(pos.x + 2, pos.y, pos.z, r, g, b)
            };
            break;

        case L:
            cubes = {
                Cube(pos.x - 1, pos.y, pos.z, r, g, b),
                Cube(pos.x, pos.y, pos.z, r, g, b),
                Cube(pos.x + 1, pos.y, pos.z, r, g, b),
                Cube(pos.x + 1, pos.y + 1, pos.z, r, g, b)
            };
            break;

        case J:
            cubes = {
                Cube(pos.x - 1, pos.y, pos.z, r, g, b),
                Cube(pos.x, pos.y, pos.z, r, g, b),
                Cube(pos.x + 1, pos.y, pos.z, r, g, b),
                Cube(pos.x - 1, pos.y + 1, pos.z, r, g, b)
            };
            break;

        case O:
            cubes = {
                Cube(pos.x, pos.y, pos.z, r, g, b),
                Cube(pos.x + 1, pos.y, pos.z, r, g, b),
                Cube(pos.x, pos.y + 1, pos.z, r, g, b),
                Cube(pos.x + 1, pos.y + 1, pos.z, r, g, b)
            };
            break;

        case T:
            cubes = {
                Cube(pos.x - 1, pos.y, pos.z, r, g, b),
                Cube(pos.x, pos.y, pos.z, r, g, b),
                Cube(pos.x + 1, pos.y, pos.z, r, g, b),
                Cube(pos.x, pos.y + 1, pos.z, r, g, b)
            };
            break;

        case S:
            cubes = {
                Cube(pos.x, pos.y, pos.z, r, g, b),
                Cube(pos.x + 1, pos.y, pos.z, r, g, b),
                Cube(pos.x, pos.y + 1, pos.z, r, g, b),
                Cube(pos.x - 1, pos.y + 1, pos.z, r, g, b)
            };
            break;

        case Z:
            cubes = {
                Cube(pos.x - 1, pos.y, pos.z, r, g, b),
                Cube(pos.x, pos.y, pos.z, r, g, b),
                Cube(pos.x, pos.y + 1, pos.z, r, g, b),
                Cube(pos.x + 1, pos.y + 1, pos.z, r, g, b)
            };
            break;
        }
    }

    void move(float dx, float dy, float dz) {
    // Simulation de la position
    float minX = -5.0f, maxX = 5.0f;
    float minY = 0.0f;

    // Vérifie que les cubes restent dans la zone
    bool canMove = true;
    for (auto& c : cubes) {
        float nx = c.x + dx;
        float ny = c.y + dy;
        if (nx < minX || nx > maxX || ny < minY) {
            canMove = false;
            break;
        }
    }

    if (!canMove) return;

    // Déplace la pièce
    position += glm::vec3(dx, dy, dz);
    for (auto& c : cubes) {
        c.x += dx;
        c.y += dy;
        c.z += dz;
    }
}



    void draw() const {
        for (const auto& c : cubes)
            c.draw();
    }
};
