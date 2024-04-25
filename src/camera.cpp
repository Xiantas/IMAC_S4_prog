#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 dir, float fov)
    : pos(pos)
    , dir(dir)
    , fov(fov)
{}


auto Camera::projMatrix() const -> glm::mat4 {
    return glm::perspective(glm::radians(fov), 1.0f, 0.1f, 100.f);
}

auto Camera::viewMatrix() const -> glm::mat4 {
    glm::vec3 up(0.0, 1.0, 0.0);
    return glm::lookAt(this->pos, this->pos + this->dir, up);
}
