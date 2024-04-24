#pragma once
#include <span>
#include "glm/glm.hpp"
#include "maths.h"
class Boid {
public :
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;

    float speed;
    float detectionRadius;
    float dodgeRadius;

    void update(std::span<Boid> crowd);

    void updateColor(bool isTurtlePresent, double p) {
        if (isTurtlePresent && maths::bernoulli(p)) {
            color = glm::vec3(0.5, 0.3, 0.7); // Changez à la couleur désirée
        }
    }
};
