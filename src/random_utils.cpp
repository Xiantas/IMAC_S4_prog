#include "random_utils.h"

#include <random>

#include <glm/glm.hpp>

namespace rng {

auto uniform0to1() -> float {
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto distrib = std::uniform_real_distribution<float>{0.0, 1.0};

    return distrib(gen);
}

auto duniform0to1() -> double {
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto distrib = std::uniform_real_distribution<double>{0.0, 1.0};

    return distrib(gen);
}

auto pointInRect(glm::vec2 p1, glm::vec2 p2) -> glm::vec2 {
    return glm::vec2(
        p1.x + uniform0to1() * (p2.x - p1.x),
        p1.y + uniform0to1() * (p2.y - p1.y));
}

} // namespace random
