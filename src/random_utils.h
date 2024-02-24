#pragma once

#include <glm/glm.hpp>

namespace rng {

float uniform0to1();
double duniform0to1();

glm::vec2 pointInRect(glm::vec2 p1, glm::vec2 p2);

} // namespace random
