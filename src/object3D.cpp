#include "object3D.h"

#include <mesh.h>
#include <openGL/program.h>
#include <optional>
#include <filesystem>
#include "renderer.h"

namespace _fs = std::filesystem;

Object3D::Object3D(VaoType vaoType)
    : mesh(vaoType)
{}

auto Object3D::loadObj(_fs::path const &path) -> Object3D& {
    this->mesh.loadObj(path);

    return *this;
}

auto Object3D::loadTexture(_fs::path const &path) -> Object3D& {
    this->mesh.loadTexture(path);

    return *this;
}

auto Object3D::setProgram(
    Renderer &renderer,
    _fs::path const &vert,
    _fs::path const &frag
) -> Object3D& {
    this->glProgramIndex = std::make_optional(renderer.getProgramIndex(vert, frag));

    return *this;
}

auto Object3D::setTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) -> Object3D& {
    this->mesh.setTransform(pos, rot, scale);

    return *this;
}

void Object3D::setInstanceData(std::span<glm::vec3> slice) {
    this->mesh.setInstanceData(slice);
}

void Object3D::render(Renderer const &renderer) {
    if (!this->glProgramIndex) {
        return;
    }

    GLprogram const &renderProgram = renderer.getProgramRef(*(this->glProgramIndex));
    renderProgram.use();
    glm::mat4 projM = renderer.camera.projMatrix();
    glm::mat4 viewM = renderer.camera.viewMatrix();
    this->mesh.render(renderProgram, projM, viewM);
}
