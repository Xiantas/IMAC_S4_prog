#include "boid.h"

#include <span>

#include "glm/glm.hpp"

void Boid::update(std::span<Boid> crowd) {
    glm::vec2 massCenter{0, 0}, averageDir{0, 0}, repulsion{0, 0};

    float count(0);
    for (Boid const &boid : crowd) {
        auto link = this->position - boid.position;
        auto distance = glm::length(link);
        if (this != &boid && distance <= detectionRadius) {
            massCenter += boid.position;
            averageDir += boid.direction;
            repulsion += distance < dodgeRadius
                ? link/(distance*distance)
                : glm::vec2(0, 0);
            count++;
        }
    }

    auto massSteer = glm::normalize(massCenter/count - this->position);
    averageDir = glm::normalize(averageDir);
    repulsion = glm::normalize(repulsion);

    auto redirect = glm::normalize(massSteer + 1.5f * averageDir + repulsion);

    auto new_dir = this->direction + 0.3f * glm::normalize(-3.5f * this->direction + redirect);

    this->direction = glm::length(new_dir) > 0 ? new_dir : this->direction;
    this->position += speed * this->direction;
}
