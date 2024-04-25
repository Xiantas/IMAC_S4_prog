#pragma once

#include <openGL/program.h>
#include <openGL/objects.h>
#include <filesystem>
#include "camera.h"
#include <span>

#include <glm/glm.hpp>

class Mesh {
public:
    Mesh() = delete;
    Mesh(VaoType vaoType);

    void loadObj(std::filesystem::path const &path);
    void loadTexture(std::filesystem::path const &path);
    void setTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
    void render(GLprogram const &renderProgram, glm::mat4 const &projM, glm::mat4 const &viewM) const;

    void setInstanceData(std::span<glm::vec3> slice);

private:
    GLvao vao;
    GLtexture texture;
    glm::mat4 modelM;
};
