#ifndef UI_OVERLAY_H
#define UI_OVERLAY_H

#include "Cube.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class UIOverlay {
public:
    UIOverlay();
    ~UIOverlay();
    
    void renderStartMessage(const glm::mat4& view, const glm::mat4& projection);
    void renderGameOverMessage(const glm::mat4& view, const glm::mat4& projection, int score, int lines);
    void renderControls(const glm::mat4& view, const glm::mat4& projection);

private:
    void renderLetter(char letter, float x, float y, const glm::vec3& color, 
                     const glm::mat4& view, const glm::mat4& projection);
    void renderString(const std::string& text, float x, float y, const glm::vec3& color,
                     const glm::mat4& view, const glm::mat4& projection);
    
    std::vector<Cube*> uiCubes;
    void clearUICubes();
};

#endif