#pragma once

#include <openGL/program.h>
#include <openGL/objects.h>
#include <filesystem>

#include <glm/glm.hpp>

class Mesh {
public:
    Mesh() = delete;
    Mesh(VaoType vaoType);

    void loadObj(std::filesystem::path const &path);
    void loadTexture(std::filesystem::path const &path);

private:
    GLvao vao;
    GLtexture texture;
};
