#pragma once

#include <span>

#include "glm/glm.hpp"

class Obstacle {
public:
    Obstacle(glm::vec2 position, float radius) : position(position), radius(radius) {}
    
    auto isInside(glm::vec2 point) const -> bool {
        return glm::length(point - position) <= radius;
    }

private:
    glm::vec2 position;
    float radius;
};
