#include "boid.h"

#include <span>

#include "p6/p6.h"

void Boid::update(std::span<Boid> crowd) {
    glm::vec2 massCenter{0, 0}, averageDir{0, 0}, repulsion{0, 0};

    for (Boid const &boid : crowd) {
        auto link = this->position - boid.position;
        auto distance = glm::length(link);
        if (this != &boid && distance <= detectionRadius) {
            massCenter += boid.position;
            averageDir += boid.direction;
            repulsion += link/(distance*distance);
        }
    }

    auto massSteer = glm::normalize(massCenter - this->position);
    averageDir = glm::normalize(averageDir);
    repulsion = repulsion/static_cast<float>(crowd.size());

    auto redirect = massSteer + averageDir + repulsion;

    auto new_dir = this->direction + redirect;

    this->direction = glm::normalize(glm::length(new_dir) > 0 ? new_dir : this->direction);
    this->position += speed * this->direction;
}
