#pragma once

#include <openGL/program.h>

#include <filesystem>
#include <glm/glm.hpp>
#include <optional>
#include <tuple>
#include <vector>

#include "camera.h"

class Renderer {
public:
    Renderer(Camera camera);
    Renderer(Renderer const& renderer) = delete;
    Renderer(Renderer&& renderer) = delete;

    auto isProgramLoaded(
        std::filesystem::path const& vert,
        std::filesystem::path const& frag
    ) const -> std::optional<size_t>;
    auto getProgramIndex(
        std::filesystem::path const& vert,
        std::filesystem::path const& frag
    ) -> size_t;
    auto getProgramRef(size_t index) const -> GLprogram const&;

    Camera camera;

private:
    std::vector<std::tuple<
        std::filesystem::path,
        std::filesystem::path,
        GLprogram>>
        programs;
};
