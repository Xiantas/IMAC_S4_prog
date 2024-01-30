#pragma once

#include <span>

#include "p6/p6.h"

class Boid {
public :
    glm::vec2 position;
    glm::vec2 direction;

    float speed;
    float detectionRadius;

    void update(std::span<Boid> crowd);
};
