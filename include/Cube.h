#pragma once
#include <glad/glad.h>

class Cube {
public:
    float x, y, z;
    float r, g, b;

    Cube(float x_, float y_, float z_, float r_=0.3f, float g_=0.5f, float b_=1.0f)
        : x(x_), y(y_), z(z_), r(r_), g(g_), b(b_) {}

    void draw() const {
        glPushMatrix();
        glTranslatef(x, y, z);

        // ---- Remplissage du cube ----
        glColor3f(r, g, b);
        glBegin(GL_QUADS);
        // Avant
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);
        // Arrière
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);
        // Côtés
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);

        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f,  0.5f);
        glVertex3f(0.5f,  0.5f,  0.5f);
        glVertex3f(0.5f,  0.5f, -0.5f);
        // Haut
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f( 0.5f, 0.5f, -0.5f);
        glVertex3f( 0.5f, 0.5f,  0.5f);
        glVertex3f(-0.5f, 0.5f,  0.5f);
        // Bas
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glEnd();

        // ---- Contours noirs ----
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        // Relier tous les coins du cube
        float v[8][3] = {
            {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
            {0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
            {-0.5f, -0.5f,  0.5f}, {0.5f, -0.5f,  0.5f},
            {0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}
        };

        int edges[12][2] = {
            {0,1},{1,2},{2,3},{3,0},
            {4,5},{5,6},{6,7},{7,4},
            {0,4},{1,5},{2,6},{3,7}
        };

        for (auto& e : edges) {
            glVertex3fv(v[e[0]]);
            glVertex3fv(v[e[1]]);
        }
        glEnd();

        glPopMatrix();
    }
};
