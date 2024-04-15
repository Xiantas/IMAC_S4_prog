#include <cstdlib>
#include <iostream>
#include <span>
#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <exe_path/exe_path.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

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

auto main() -> int {
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    if (!glfwInit()) {
        std::cout << "Couldn't init glfw\n";
        return -1;
    }

    glfwSetErrorCallback(onError);

    GLFWwindow* window = basicWindowInit(1000, 1000, std::string("Machin"));

    GLshader vsShader(GL_VERTEX_SHADER), fsShader(GL_FRAGMENT_SHADER);
    vsShader.setCodeFromFile("shaders/boids.vs.glsl");
    fsShader.setCodeFromFile("shaders/boids.fs.glsl");

    GLprogram renderProgram;
    renderProgram
        .addShader(vsShader)
        .addShader(fsShader)
        .link();

    renderProgram.use();

    GLuint singularBoidVBO = createSingularBoidVBO();
    GLuint boidsDisplacementVBO = createBoidsDisplacementVBO(N);

    GLuint boidsVAO;
    glGenVertexArrays(1, &boidsVAO);

    glBindVertexArray(boidsVAO);

    setupVertexAttributes(singularBoidVBO, boidsDisplacementVBO);

    //    glBindVertexArray(0);

    std::vector<Boid> crowd(N);
    for (Boid& boid : crowd) {
        glm::vec2 point = rng::pointInRect(glm::vec2(-1.f, -1.f), glm::vec2(1.f, 1.f));
        boid = Boid{
            .position = point,
            .direction = point,
            .speed = 0.015,
            .detectionRadius = .25,
            .dodgeRadius = .13,
        };
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        for (Boid& boid : crowd) {
            boid.update(std::span(crowd));
            if (boid.position.x < -1)
                boid.position.x = 0.99;
            if (boid.position.x > 1)
                boid.position.x = -0.99;
            if (boid.position.y < -1)
                boid.position.y = 0.99;
            if (boid.position.y > 1)
                boid.position.y = -0.99;
        }

        glBindBuffer(GL_ARRAY_BUFFER, boidsDisplacementVBO);
        std::vector<glm::vec2> drawData(2 * N);

        for (size_t i = 0; i < N; ++i) {
            drawData[2 * i] = crowd[i].position;
            drawData[2 * i + 1] = crowd[i].direction;
        }
        glBufferData(GL_ARRAY_BUFFER, drawData.size() * sizeof(glm::vec2), drawData.data(), GL_DYNAMIC_DRAW);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, N);

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &boidsDisplacementVBO);
    glDeleteBuffers(1, &singularBoidVBO);
    glDeleteVertexArrays(1, &boidsVAO);

    glfwTerminate();
}
