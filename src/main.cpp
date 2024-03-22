#include <cstdlib>
#include <vector>
#include <span>

#include <iostream>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <exe_path/exe_path.h>

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "window.h"
#include "openGL/program.h"
#include "boid.h"
#include "file_utils.h"
#include "random_utils.h"

constexpr size_t N = 500;

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

    GLFWwindow *window = basicWindowInit(1000, 1000, std::string("Machin"));

    GLshader vsShader(GL_VERTEX_SHADER), fsShader(GL_FRAGMENT_SHADER);
    vsShader.setCodeFromFile("shaders/boids.vs.glsl");
    fsShader.setCodeFromFile("shaders/boids.fs.glsl");

    GLprogram renderProgram;
    renderProgram
        .addShader(vsShader)
        .addShader(fsShader)
        .link();

    vsShader.setCodeFromFile("shaders/fade.vs.glsl");
    fsShader.setCodeFromFile("shaders/fade.fs.glsl");

    GLprogram fadeProgram;
    fadeProgram
        .addShader(vsShader)
        .addShader(fsShader)
        .link();

    GLuint singularBoidVBO;
    {
        glGenBuffers(1, &singularBoidVBO);
        glBindBuffer(GL_ARRAY_BUFFER, singularBoidVBO);
        GLfloat vertices[] = {
             0.03f,  0.f,
            -0.01f,  0.01f,
            -0.01f, -0.01f,
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    std::vector<glm::vec2> drawData(2*N);
    GLuint boidsDisplacementVBO;
    glGenBuffers(1, &boidsDisplacementVBO);


    GLuint boidsVAO;
    glGenVertexArrays(1, &boidsVAO);

    glBindVertexArray(boidsVAO);

    glBindBuffer(GL_ARRAY_BUFFER, singularBoidVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, boidsDisplacementVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), nullptr);
    glVertexAttribDivisor(1, 1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
    glVertexAttribDivisor(2, 1);


    std::vector<Boid> crowd(N);
    for (Boid &boid : crowd) {
        glm::vec2 point = rng::pointInRect(glm::vec2(-1.f, -1.f), glm::vec2(1.f, 1.f));
        boid = Boid {
            .position = point,
            .direction = point,
            .speed = 0.005,
            .detectionRadius = .25,
            .dodgeRadius = .13,
        };
    }

    GLuint squareVBO;
    {
        glGenBuffers(1, &squareVBO);
        glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
        GLfloat vertices[] = {
            -1.0f, -1.0f,
            1.0f, -1.0f,
            -1.0f, 1.0f,
            1.0f, 1.0f,
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLuint squareVAO;
    glGenVertexArrays(1, &squareVAO);

    glBindVertexArray(squareVAO);

    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, boidsDisplacementVBO);

    glBindVertexArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        for (Boid &boid : crowd) {
            boid.update(std::span(crowd));
            if (boid.position.x < -1) boid.position.x =  0.99;
            if (boid.position.x >  1) boid.position.x = -0.99;
            if (boid.position.y < -1) boid.position.y =  0.99;
            if (boid.position.y >  1) boid.position.y = -0.99;
        }

        for (size_t i = 0; i < N; ++i) {
            drawData[2*i] = crowd[i].position;
            drawData[2*i+1] = crowd[i].direction;
        }
        glBufferData(GL_ARRAY_BUFFER, drawData.size()*sizeof(glm::vec2), drawData.data(), GL_DYNAMIC_DRAW);

//		glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(squareVAO);
        fadeProgram.use();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindVertexArray(boidsVAO);
        renderProgram.use();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, N);


        glfwSwapBuffers(window);
    }



    glDeleteBuffers(1, &boidsDisplacementVBO);
    glDeleteBuffers(1, &singularBoidVBO);
    glDeleteVertexArrays(1, &boidsVAO);

    glfwTerminate(); 
}
