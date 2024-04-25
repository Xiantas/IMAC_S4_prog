#pragma once

#include <mesh.h>
#include <openGL/program.h>
#include <optional>
#include <filesystem>
#include "rendering.h"

class Object3D {
public:
    Object3D() = delete;
    Object3D(VaoType vaoType);

    void loadObj(std::filesystem::path const &path);
    void loadTexture(std::filesystem::path const &path);
    void setProgram(
        std::filesystem::path const &vert,
        std::filesystem::path const &frag,
        Renderer &renderer);

private:
    Mesh mesh;
    std::optional<size_t> glProgramIndex;
};
