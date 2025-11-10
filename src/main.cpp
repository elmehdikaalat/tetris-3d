#include "GameField.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

GameField* gameField = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (gameField) {
            switch (key) {
                case GLFW_KEY_A:
                case GLFW_KEY_LEFT:
                    gameField->moveCurrentPiece(-1, 0);
                    break;
                case GLFW_KEY_E:
                case GLFW_KEY_RIGHT:
                    gameField->moveCurrentPiece(1, 0);
                    break;
                case GLFW_KEY_S:
                case GLFW_KEY_DOWN:
                    gameField->dropCurrentPiece();
                    break;
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, true);
                    break;
            }
        }
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tetris 3D", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Create game field
    gameField = new GameField();

    auto lastTime = std::chrono::high_resolution_clock::now();
    float dropTimer = 0.0f;
    const float DROP_INTERVAL = 0.4f; // Faster drop interval (was 1.0f)

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        dropTimer += deltaTime;
        if (dropTimer >= DROP_INTERVAL) {
            gameField->update();
            dropTimer = 0.0f;
        }

        // Set white background
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gameField->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete gameField;
    glfwTerminate();
    return 0;
}