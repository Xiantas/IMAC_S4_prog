#pragma once

#include <glm/glm.hpp>

namespace rng {

auto uniform0to1() -> float;
auto duniform0to1() -> double;

auto pointInRect(glm::vec2 p1, glm::vec2 p2) -> glm::vec2;
auto pointInSphere(float radius) -> glm::vec3;
} // namespace random
