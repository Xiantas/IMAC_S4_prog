#pragma once

#include <mesh.h>
#include <openGL/program.h>
#include <optional>
#include <filesystem>
#include "renderer.h"

class Object3D {
public:
    Object3D() = delete;
    Object3D(VaoType vaoType);

    auto loadObj(std::filesystem::path const &path) -> Object3D&;
    auto loadTexture(std::filesystem::path const &path) -> Object3D&;
    auto setProgram(
        Renderer &renderer,
        std::filesystem::path const &vert,
        std::filesystem::path const &frag
    ) -> Object3D&;

    void render(Renderer const &renderer);

private:
    Mesh mesh;
    std::optional<size_t> glProgramIndex;
};
