#include <cstdlib>
#include <iostream>
#include <span>
#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DOCTEST_CONFIG_IMPLEMENT
#include "boid.h"
#include "doctest/doctest.h"
#include "file_utils.h"
#include "openGL/arrays.h"
#include "openGL/program.h"
#include "random_utils.h"
#include "window.h"

constexpr size_t N = 100;

void onError(int error, const char* description) {
    std::cout << "glfw error #" << error << ": " << description << "\n";
}

struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

std::vector<ShapeVertex> createSphereVertices(float radius, unsigned int rings, unsigned int sectors) {
    std::vector<ShapeVertex> vertices;
    float const R = 1.0f / (float)(rings - 1);
    float const S = 1.0f / (float)(sectors - 1);

    for (int r = 0; r < rings; ++r) {
        for (int s = 0; s < sectors; ++s) {
            float const y = sin(-glm::pi<float>() / 2 + glm::pi<float>() * r * R);
            float const x = cos(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
            float const z = sin(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);

            ShapeVertex vertex;
            vertex.position = glm::vec3(x, y, z) * radius;
            vertex.normal = glm::normalize(vertex.position);
            vertex.texCoords = glm::vec2(s * S, r * R);

            vertices.push_back(vertex);
        }
    }
    return vertices;
}

int main() {
    if (!glfwInit()) {
        std::cout << "Couldn't init glfw\n";
        return -1;
    }

    glfwSetErrorCallback(onError);
    GLFWwindow* window = basicWindowInit(1000, 1000, std::string("Machin"));
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }

    GLshader vsShader(GL_VERTEX_SHADER), fsShader(GL_FRAGMENT_SHADER);
    vsShader.setCodeFromFile("shaders/3D.vs.glsl");
    fsShader.setCodeFromFile("shaders/normals.fs.glsl");

    GLprogram renderProgram;
    renderProgram
        .addShader(vsShader)
        .addShader(fsShader)
        .link();
    renderProgram.use();

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    std::vector<ShapeVertex> sphereVertices = createSphereVertices(1.0f, 20, 20);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ShapeVertex) * sphereVertices.size(), sphereVertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void*)offsetof(ShapeVertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void*)offsetof(ShapeVertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void*)offsetof(ShapeVertex, texCoords));

    glEnable(GL_DEPTH_TEST);
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), 1.0f, 0.1f, 100.f);
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    GLint locMVPMatrix = glGetUniformLocation(renderProgram.getID(), "uMVPMatrix");
    GLint locMVMatrix = glGetUniformLocation(renderProgram.getID(), "uMVMatrix");
    GLint locNormalMatrix = glGetUniformLocation(renderProgram.getID(), "uNormalMatrix");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(locMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(locMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(locNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, sphereVertices.size());
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}
