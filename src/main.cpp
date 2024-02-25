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
#include "boid.h"
#include "file_utils.h"
#include "random_utils.h"

constexpr size_t N = 500;

void onError(int error, const char* description) {
    std::cout << "glfw error #" << error << ": " << description << "\n";
}

int main() {
    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    if (!glfwInit()) {
        std::cout << "Couldn't init glfw\n";
        return -1;
    }

	glfwSetErrorCallback(onError);

    GLFWwindow *window = basicWindowInit(1000, 1000, std::string("Machin"));

    std::string vsString, fsString;
    file_utils::read(vsString, "../shaders/boids.vs.glsl");
    file_utils::read(fsString, "../shaders/boids.fs.glsl");
    char const
        *vsChars = vsString.c_str(),
        *fsChars = fsString.c_str();

    GLuint vsShader, fsShader, programID;

    vsShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsShader, 1, &vsChars, nullptr);
    glCompileShader(vsShader);
    // TODO finish this : refactor + error handling

    fsShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsShader, 1, &fsChars, nullptr);
    glCompileShader(fsShader);

    programID = glCreateProgram();
    glAttachShader(programID, vsShader);
    glAttachShader(programID, fsShader);
    glLinkProgram(programID);

    glUseProgram(programID);

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

//    glBindVertexArray(0);

    std::vector<Boid> crowd(N);
    for (Boid &boid : crowd) {
        glm::vec2 point = rng::pointInRect(glm::vec2(-1.f, -1.f), glm::vec2(1.f, 1.f));
        boid = Boid {
            .position = point,
            .direction = point,
            .speed = 0.0047,
            .detectionRadius = .25,
            .dodgeRadius = .13,
        };
    }

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

		glClear(GL_COLOR_BUFFER_BIT);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 3, N);


        glfwSwapBuffers(window);
    }



    glDeleteBuffers(1, &boidsDisplacementVBO);
    glDeleteBuffers(1, &singularBoidVBO);
    glDeleteVertexArrays(1, &boidsVAO);
    glDeleteProgram(programID);
    glDeleteShader(vsShader);
    glDeleteShader(fsShader);

    glfwTerminate(); 
}
