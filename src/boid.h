#pragma once

#include <span>

#include "glm/glm.hpp"

class Boid {
public :
    glm::vec2 position;
    glm::vec2 direction;

    float speed;
    float detectionRadius;
    float dodgeRadius;

    void update(std::span<Boid> crowd);
};
