#pragma once

#include <vector>
#include <openGL/program.h>
#include <glm/glm.hpp>
#include <tuple>
#include <filesystem>

class Camera {
private:
    glm::vec3 pos;
    glm::vec3 dir;

    float angle;
};

class Renderer {
public:
    Renderer();
    Renderer(Renderer const &renderer) = delete;
    Renderer(Renderer &&renderer) = delete;

    auto isProgramLoaded(std::filesystem::path) -> bool;

    Camera camera;
private:
    std::vector<std::tuple<std::filesystem::path, GLprogram>> programs;
};
