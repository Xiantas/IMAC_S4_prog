#pragma once

#include <openGL/program.h>

#include <filesystem>
#include <glm/glm.hpp>
#include <optional>
#include <tuple>
#include <vector>

class Camera {
private:
    glm::vec3 pos;
    glm::vec3 dir;

    float angle;
};

class Renderer {
public:
    Renderer();
    Renderer(Renderer const& renderer) = delete;
    Renderer(Renderer&& renderer) = delete;

    auto isProgramLoaded(
        std::filesystem::path const& vert,
        std::filesystem::path const& frag
    ) -> std::optional<size_t>;
    auto getProgram(
        std::filesystem::path const& vert,
        std::filesystem::path const& frag
    ) -> size_t;
    void useProgram(size_t index);

    Camera camera;

private:
    std::vector<std::tuple<
        std::filesystem::path,
        std::filesystem::path,
        GLprogram>>
        programs;
};
