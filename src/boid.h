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

    void updateColor(bool isTurtlePresent) {
    if (isTurtlePresent && maths::bernoulli(0.5)) { // Proba de 50%
            color = glm::vec3(0.5f, 0.5f, 1.0f); // Assombrir avec un ton bleu
        } else {
            color = glm::vec3(1.0f, 1.0f, 1.0f); // Couleur défaut blanche
        }
    }
};
