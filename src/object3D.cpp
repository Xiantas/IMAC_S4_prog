#include "object3D.h"

#include <mesh.h>
#include <openGL/program.h>
#include <optional>
#include <filesystem>
#include "rendering.h"

namespace _fs = std::filesystem;

Object3D::Object3D(VaoType vaoType)
    : mesh(vaoType)
{}

void Object3D::loadObj(_fs::path const &path) {
    this->mesh.loadObj(path);
}

void Object3D::setProgram(_fs::path const &vert, _fs::path const &frag, Renderer &renderer) {
    this->glProgramIndex = std::make_optional(renderer.getProgram(vert, frag));
}
