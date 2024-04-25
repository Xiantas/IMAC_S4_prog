#pragma once

#include <glm/glm.hpp>

struct Camera {
    Camera(glm::vec3 pos, glm::vec3 dir, float fov);

    glm::vec3 pos;
    glm::vec3 dir;

    float fov;

    auto projMatrix() const -> glm::mat4;
    auto viewMatrix() const -> glm::mat4;
};
