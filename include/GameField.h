#pragma once
#include <glad/glad.h>

class GameField {
public:
    int width, depth, height;

    GameField(int w = 10, int d = 4, int h = 12)
        : width(w), depth(d), height(h) {}

    void draw() const {
        glColor3f(0.3f, 0.3f, 0.3f);
        glLineWidth(1.0f);

        // ---- Sol ----
        glBegin(GL_LINES);
        for (int x = -width / 2; x <= width / 2; ++x) {
            glVertex3f((float)x, 0.0f, -(float)depth / 2);
            glVertex3f((float)x, 0.0f, (float)depth / 2);
        }
        for (int z = -depth / 2; z <= depth / 2; ++z) {
            glVertex3f(-(float)width / 2, 0.0f, (float)z);
            glVertex3f((float)width / 2, 0.0f, (float)z);
        }
        glEnd();

        // ---- Murs ----
        glColor3f(0.15f, 0.15f, 0.15f);
        for (int y = 0; y <= height; ++y) {
            glBegin(GL_LINE_LOOP);
            glVertex3f(-(float)width / 2, (float)y, -(float)depth / 2);
            glVertex3f((float)width / 2, (float)y, -(float)depth / 2);
            glVertex3f((float)width / 2, (float)y, (float)depth / 2);
            glVertex3f(-(float)width / 2, (float)y, (float)depth / 2);
            glEnd();
        }

        // ---- Coins verticaux ----
        glBegin(GL_LINES);
        for (int x = -width / 2; x <= width / 2; x += width) {
            for (int z = -depth / 2; z <= depth / 2; z += depth) {
                glVertex3f((float)x, 0.0f, (float)z);
                glVertex3f((float)x, (float)height, (float)z);
            }
        }
        glEnd();
    }
};
