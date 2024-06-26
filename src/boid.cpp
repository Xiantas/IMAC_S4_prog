#include "boid.h"

void Boid::update(std::span<Boid> crowd) {
    glm::vec3 massCenter{0, 0, 0}, averageDir{0, 0, 0}, repulsion{0, 0, 0};

    float count(0);
    for (Boid const &boid : crowd) {
        auto link = this->position - boid.position;
        auto distance = glm::length(link);
        if (this != &boid && distance <= detectionRadius) {
            massCenter += boid.position;
            averageDir += boid.direction;
            repulsion += distance < this->dodgeRadius
                ? link/(distance*distance)
                : glm::vec3(0, 0, 0);
            count++;
        }
    }

    auto massSteer = glm::normalize(massCenter/count - this->position);
    averageDir = glm::normalize(averageDir);
    repulsion = repulsion/count;

    auto redirect = glm::normalize(massSteer + 1.5f * averageDir + 2.0f * repulsion);

    auto new_dir = glm::normalize(this->direction + 0.1f * glm::normalize(-3.5f * this->direction + redirect));

    this->direction = glm::length(new_dir) > 0 ? new_dir : this->direction;
    this->position += speed * this->direction;
}
