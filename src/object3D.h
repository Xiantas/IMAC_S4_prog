#pragma once

#include <mesh.h>
#include <openGL/program.h>
#include <optional>
#include <filesystem>
#include "rendering.h"

class Object3D {
public:
    Object3D() {}

    void loadFromFile(std::filesystem::path const &path);
    void setProgram(std::filesystem::path const &path);
    void setProgram(std::filesystem::path const &path, Renderer &renderer);

private:
    Mesh mesh;
    std::optional<size_t> glProgramIndex;
};
