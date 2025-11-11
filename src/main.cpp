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
            GameState state = gameField->getGameState();
            
            switch (key) {
                case GLFW_KEY_SPACE:
                    if (state == GameState::WAITING_TO_START) {
                        gameField->startGame();
                    } else if (state == GameState::GAME_OVER) {
                        gameField->resetGame();
                    }
                    break;
                    
                case GLFW_KEY_A:
                case GLFW_KEY_LEFT:
                    if (state == GameState::PLAYING) {
                        gameField->moveCurrentPiece(-1, 0);
                    }
                    break;
                    
                case GLFW_KEY_E:
                case GLFW_KEY_RIGHT:
                    if (state == GameState::PLAYING) {
                        gameField->moveCurrentPiece(1, 0);
                    }
                    break;
                    
                case GLFW_KEY_S:
                case GLFW_KEY_DOWN:
                    if (state == GameState::PLAYING) {
                        gameField->dropCurrentPiece();
                    }
                    break;
                    
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, true);
                    break;
            }
        }
    }
}

void printInstructions() {
    std::cout << "\n=== TETRIS 3D ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  SPACE: Start game / Restart when game over" << std::endl;
    std::cout << "  A/LEFT: Move left" << std::endl;
    std::cout << "  E/RIGHT: Move right" << std::endl;
    std::cout << "  S/DOWN: Drop piece" << std::endl;
    std::cout << "  ESC: Quit" << std::endl;
    std::cout << "\nPress SPACE to start!" << std::endl;
    std::cout << "==================\n" << std::endl;
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

    glEnable(GL_DEPTH_TEST);
    
    printInstructions();
    gameField = new GameField();

    auto lastTime = std::chrono::high_resolution_clock::now();
    float dropTimer = 0.0f;
    const float DROP_INTERVAL = 0.4f;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        // Only update game logic when playing
        if (gameField->getGameState() == GameState::PLAYING) {
            dropTimer += deltaTime;
            if (dropTimer >= DROP_INTERVAL) {
                gameField->update();
                dropTimer = 0.0f;
            }
        }

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