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

namespace _fs = std::filesystem;

void Mesh::loadFromFile(_fs::path const &path) {
    tinyobj::attrib_t                attributs;
    std::vector<tinyobj::shape_t>    formes;
    std::vector<tinyobj::material_t> materiaux;
    std::string                      erreur;
    std::string                      avertissement;

    std::vector<VertexData> vertices;

    // Charge le fichier objet
    const bool loaded = tinyobj::LoadObj(
            &attributs, &formes, &materiaux,
            &avertissement, &erreur, path.c_str());
    if (!loaded)
    {
        std::cout << "Coudn't load \"" << path << "\" : " << std::endl;
    }

    // Parcourt les formes du modèle
    for (const auto& forme : formes)
    {
        for (const auto& index : forme.mesh.indices)
        {
            glm::vec3 position(
                attributs.vertices[3 * index.vertex_index + 0],
                attributs.vertices[3 * index.vertex_index + 1],
                attributs.vertices[3 * index.vertex_index + 2]
            );

            glm::vec3 normal(
                attributs.normals[3 * index.normal_index + 0],
                attributs.normals[3 * index.normal_index + 1],
                attributs.normals[3 * index.normal_index + 2]
            );

            glm::vec2 texCoord(
                attributs.texcoords[2 * index.texcoord_index + 0],
                attributs.texcoords[2 * index.texcoord_index + 1]
            );

            vertices.emplace_back(position, normal, texCoord);
        }
    }

    this->vao.setVboMain(std::span(vertices));

    std::cout << "\"" << path << "\" loaded successfully\n";
}
