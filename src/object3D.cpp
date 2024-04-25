#include "object3D.h"

#include <mesh.h>
#include <openGL/program.h>
#include <optional>
#include <filesystem>
#include "rendering.h"

void Object3D::loadFromFile(std::filesystem::path const &path) {
    this->mesh.loadFromFile(path);
}

void Object3D::setProgram(std::filesystem::path const &path, Renderer &renderer) {
    this->glProgramIndex = std::make_optional(renderer.getProgram(path));
}
