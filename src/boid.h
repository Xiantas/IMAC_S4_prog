#pragma once

#include <span>

#include "glm/glm.hpp"

class Boid {
public :
    glm::vec3 position;
    glm::vec3 direction;

    float speed;
    float detectionRadius;
    float dodgeRadius;

    void update(std::span<Boid> crowd);
};
