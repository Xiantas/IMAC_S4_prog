#include <mesh.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader.h>
#include <string>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "file_utils.h"
#include <iostream>
#include "vertexData.h"
#include <span>
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

namespace _fs = std::filesystem;

Mesh::Mesh(VaoType vaoType) : vao(vaoType) {}

void Mesh::loadObj(_fs::path const &path) {
    tinyobj::attrib_t attribs;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string error;
    std::string warning;

    std::vector<VertexData> vertices;

    auto newPath = file_utils::findPath(path);

    // Charge le fichier objet
    const bool loaded = tinyobj::LoadObj(
            &attribs, &shapes, &materials,
            &warning, &error, newPath.c_str());
    if (!loaded)
    {
        std::cout << "Coudn't load \"" << path << "\" : " << std::endl;
    }

    // Parcourt les shapes du modèle
    for (const auto& forme : shapes)
    {
        for (const auto& index : forme.mesh.indices)
        {
            glm::vec3 position(
                attribs.vertices[3 * index.vertex_index + 0],
                attribs.vertices[3 * index.vertex_index + 1],
                attribs.vertices[3 * index.vertex_index + 2]
            );

            glm::vec3 normal(
                attribs.normals[3 * index.normal_index + 0],
                attribs.normals[3 * index.normal_index + 1],
                attribs.normals[3 * index.normal_index + 2]
            );

            glm::vec2 texCoord(
                attribs.texcoords[2 * index.texcoord_index + 0],
                attribs.texcoords[2 * index.texcoord_index + 1]
            );

            vertices.emplace_back(position, normal, texCoord);
        }
    }

    this->vao.setVboMain(std::span(vertices));

    std::cout << path << " loaded successfully\n";
}

void Mesh::loadTexture(_fs::path const &path) {
    _fs::path newPath = file_utils::findPath(path);
    int width, height, nbChannels;
    unsigned char *data = stbi_load(newPath.c_str(), &width, &height, &nbChannels, 0);
    if (data) {
        this->texture.setData(width, height, nbChannels, data);
        std::cout << "texture " << path << " loaded\n";
    } else {
        std::cout << "failed to load texture " << path << '\n';
    }

    stbi_image_free(data);
}

/*
void Mesh::setTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
        this->
}
*/

void Mesh::render(GLprogram const &renderProgram, glm::mat4 const &projM, glm::mat4 const &viewM) const {
    glm::mat4 modelM(1.0); // = glm::translate(glm::mat4(1.0f), boid.position);
    glm::mat4 MVMatrix = viewM * modelM;
    glm::mat4 normalM = glm::transpose(glm::inverse(MVMatrix));
    glm::mat4 MVP = projM * MVMatrix;

    GLint locMVMatrix = glGetUniformLocation(renderProgram.getID(), "uMVMatrix");
    GLint locNormalMatrix = glGetUniformLocation(renderProgram.getID(), "uNormalMatrix");
    GLint locMVPMatrix = glGetUniformLocation(renderProgram.getID(), "uMVPMatrix");

    glUniformMatrix4fv(locMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(locNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalM));
    glUniformMatrix4fv(locMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVP));

    this->texture.bind();
    this->vao.draw();
}
