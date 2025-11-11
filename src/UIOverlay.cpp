#include "UIOverlay.h"

UIOverlay::UIOverlay() {}

UIOverlay::~UIOverlay() {
    clearUICubes();
}

void UIOverlay::clearUICubes() {
    for (Cube* cube : uiCubes) {
        delete cube;
    }
    uiCubes.clear();
}

void UIOverlay::renderLetter(char letter, float x, float y, const glm::vec3& color,
                            const glm::mat4& view, const glm::mat4& projection) {
    float scale = 0.3f;
    
    switch (letter) {
        case 'P':
            // Vertical line
            for (int i = 0; i < 5; i++) {
                uiCubes.push_back(new Cube(x, y + i * scale, 0, color));
            }
            // Top horizontal
            for (int i = 0; i < 3; i++) {
                uiCubes.push_back(new Cube(x + i * scale, y + 4 * scale, 0, color));
            }
            // Middle horizontal
            for (int i = 0; i < 2; i++) {
                uiCubes.push_back(new Cube(x + i * scale, y + 2 * scale, 0, color));
            }
            // Right vertical (top half)
            for (int i = 2; i < 5; i++) {
                uiCubes.push_back(new Cube(x + 2 * scale, y + i * scale, 0, color));
            }
            break;
            
        case 'R':
            // Vertical line
            for (int i = 0; i < 5; i++) {
                uiCubes.push_back(new Cube(x, y + i * scale, 0, color));
            }
            // Top horizontal
            for (int i = 0; i < 3; i++) {
                uiCubes.push_back(new Cube(x + i * scale, y + 4 * scale, 0, color));
            }
            // Middle horizontal
            for (int i = 0; i < 2; i++) {
                uiCubes.push_back(new Cube(x + i * scale, y + 2 * scale, 0, color));
            }
            // Right verticals
            uiCubes.push_back(new Cube(x + 2 * scale, y + 3 * scale, 0, color));
            uiCubes.push_back(new Cube(x + 2 * scale, y + 4 * scale, 0, color));
            uiCubes.push_back(new Cube(x + 2 * scale, y, 0, color));
            uiCubes.push_back(new Cube(x + scale, y, 0, color));
            break;
            
        case 'E':
            // Vertical line
            for (int i = 0; i < 5; i++) {
                uiCubes.push_back(new Cube(x, y + i * scale, 0, color));
            }
            // Horizontal lines
            for (int i = 0; i < 3; i++) {
                uiCubes.push_back(new Cube(x + i * scale, y + 4 * scale, 0, color));
                uiCubes.push_back(new Cube(x + i * scale, y + 2 * scale, 0, color));
                uiCubes.push_back(new Cube(x + i * scale, y, 0, color));
            }
            break;
            
        case 'S':
            // Top horizontal
            for (int i = 0; i < 3; i++) {
                uiCubes.push_back(new Cube(x + i * scale, y + 4 * scale, 0, color));
            }
            // Left vertical (top)
            uiCubes.push_back(new Cube(x, y + 3 * scale, 0, color));
            // Middle horizontal
            for (int i = 0; i < 3; i++) {
                uiCubes.push_back(new Cube(x + i * scale, y + 2 * scale, 0, color));
            }
            // Right vertical (bottom)
            uiCubes.push_back(new Cube(x + 2 * scale, y + scale, 0, color));
            // Bottom horizontal
            for (int i = 0; i < 3; i++) {
                uiCubes.push_back(new Cube(x + i * scale, y, 0, color));
            }
            break;
            
        case 'A':
            // Vertical lines
            for (int i = 0; i < 4; i++) {
                uiCubes.push_back(new Cube(x, y + i * scale, 0, color));
                uiCubes.push_back(new Cube(x + 2 * scale, y + i * scale, 0, color));
            }
            // Top and middle horizontal
            for (int i = 0; i < 3; i++) {
                uiCubes.push_back(new Cube(x + i * scale, y + 4 * scale, 0, color));
                uiCubes.push_back(new Cube(x + i * scale, y + 2 * scale, 0, color));
            }
            break;
            
        case 'C':
            // Vertical line
            for (int i = 1; i < 4; i++) {
                uiCubes.push_back(new Cube(x, y + i * scale, 0, color));
            }
            // Horizontal lines
            for (int i = 0; i < 3; i++) {
                uiCubes.push_back(new Cube(x + i * scale, y + 4 * scale, 0, color));
                uiCubes.push_back(new Cube(x + i * scale, y, 0, color));
            }
            break;
            
        case 'O':
            // Vertical lines
            for (int i = 1; i < 4; i++) {
                uiCubes.push_back(new Cube(x, y + i * scale, 0, color));
                uiCubes.push_back(new Cube(x + 2 * scale, y + i * scale, 0, color));
            }
            // Horizontal lines
            for (int i = 0; i < 3; i++) {
                uiCubes.push_back(new Cube(x + i * scale, y + 4 * scale, 0, color));
                uiCubes.push_back(new Cube(x + i * scale, y, 0, color));
            }
            break;
            
        case 'V':
            // Diagonal effect with cubes
            uiCubes.push_back(new Cube(x, y + 4 * scale, 0, color));
            uiCubes.push_back(new Cube(x, y + 3 * scale, 0, color));
            uiCubes.push_back(new Cube(x + scale, y + 2 * scale, 0, color));
            uiCubes.push_back(new Cube(x + scale, y + scale, 0, color));
            uiCubes.push_back(new Cube(x + scale, y, 0, color));
            uiCubes.push_back(new Cube(x + 2 * scale, y + 2 * scale, 0, color));
            uiCubes.push_back(new Cube(x + 2 * scale, y + 3 * scale, 0, color));
            uiCubes.push_back(new Cube(x + 2 * scale, y + 4 * scale, 0, color));
            break;
            
        case 'M':
            // Vertical lines
            for (int i = 0; i < 5; i++) {
                uiCubes.push_back(new Cube(x, y + i * scale, 0, color));
                uiCubes.push_back(new Cube(x + 3 * scale, y + i * scale, 0, color));
            }
            // Middle peak
            uiCubes.push_back(new Cube(x + scale, y + 3 * scale, 0, color));
            uiCubes.push_back(new Cube(x + 2 * scale, y + 3 * scale, 0, color));
            uiCubes.push_back(new Cube(x + 1.5f * scale, y + 4 * scale, 0, color));
            break;
            
        case 'G':
            // Similar to C but with horizontal line on right
            for (int i = 1; i < 4; i++) {
                uiCubes.push_back(new Cube(x, y + i * scale, 0, color));
            }
            for (int i = 0; i < 3; i++) {
                uiCubes.push_back(new Cube(x + i * scale, y + 4 * scale, 0, color));
                uiCubes.push_back(new Cube(x + i * scale, y, 0, color));
            }
            uiCubes.push_back(new Cube(x + 2 * scale, y + scale, 0, color));
            uiCubes.push_back(new Cube(x + scale, y + 2 * scale, 0, color));
            break;
    }
}

void UIOverlay::renderString(const std::string& text, float x, float y, const glm::vec3& color,
                            const glm::mat4& view, const glm::mat4& projection) {
    float charSpacing = 1.2f;
    for (size_t i = 0; i < text.length(); i++) {
        if (text[i] != ' ') {
            renderLetter(text[i], x + i * charSpacing, y, color, view, projection);
        }
    }
}

void UIOverlay::renderStartMessage(const glm::mat4& view, const glm::mat4& projection) {
    clearUICubes();
    
    glm::vec3 textColor(0.0f, 0.0f, 1.0f); // Blue
    renderString("PRESS SPACE", 1.0f, 10.0f, textColor, view, projection);
    
    // Render all UI cubes
    for (Cube* cube : uiCubes) {
        cube->render(view, projection);
    }
}

void UIOverlay::renderGameOverMessage(const glm::mat4& view, const glm::mat4& projection, int score, int lines) {
    clearUICubes();
    
    glm::vec3 gameOverColor(1.0f, 0.0f, 0.0f); // Red
    renderString("GAME OVER", 0.5f, 12.0f, gameOverColor, view, projection);
    
    glm::vec3 restartColor(0.0f, 1.0f, 0.0f); // Green
    renderString("SPACE", 2.0f, 8.0f, restartColor, view, projection);
    
    // Render all UI cubes
    for (Cube* cube : uiCubes) {
        cube->render(view, projection);
    }
}

void UIOverlay::renderControls(const glm::mat4& view, const glm::mat4& projection) {
    // Simple control indicators on the side
    glm::vec3 controlColor(0.4f, 0.4f, 0.4f); // Gray
    
    // Left side control hints - just simple markers
    for (int i = 0; i < 4; i++) {
        uiCubes.push_back(new Cube(-3, 12 - i * 2, 0, controlColor));
        uiCubes.push_back(new Cube(-2, 12 - i * 2, 0, controlColor));
    }
    
    // Render control cubes
    for (Cube* cube : uiCubes) {
        cube->render(view, projection);
    }
}