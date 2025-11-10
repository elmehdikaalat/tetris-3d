#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Camera.h"
#include "Piece.h"
#include <GameField.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "Tetris 3D", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);


    double lastFallTime = glfwGetTime();
    double fallInterval = 1.0;

    Camera camera;
    GameField field(10, 4, 12);
    Piece piece(T);

    // Boucle principale
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();

        // Gravité
        if (currentTime - lastFallTime >= fallInterval) {
            piece.move(0.0f, -1.0f, 0.0f);
            lastFallTime = currentTime;
        }

        // Déplacements
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            piece.move(-1.0f, 0.0f, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            piece.move(1.0f, 0.0f, 0.0f);

        // Rendu
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glm::mat4 projection = camera.getProjectionMatrix(800.0f / 600.0f);
        glMultMatrixf(&projection[0][0]);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glm::mat4 view = camera.getViewMatrix();
        glMultMatrixf(&view[0][0]);

        // Dessiner le plan de jeu
        field.draw();

        // Dessiner la pièce
        piece.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
        

    glfwTerminate();
    return 0;
}
