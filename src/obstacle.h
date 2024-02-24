#pragma once

#include <span>

#include "glm/glm.hpp"

class Obstacle {
public:
    Obstacle(glm::vec2 position, float radius) : position(position), radius(radius) {}
    
    bool isInside(glm::vec2 point) const {
        return glm::length(point - position) <= radius;
    }

private:
    glm::vec2 position;
    float radius;
};
