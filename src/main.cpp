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

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void processInput(GLFWwindow* window) {
    const float cameraSpeed = 0.05f; 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

int main() {
    Window window(1000, 1000, std::string("Machin"));

    GLshader vsShader(GL_VERTEX_SHADER), fsShader(GL_FRAGMENT_SHADER);
    vsShader.setCodeFromFile("shaders/3D.vs.glsl");
    fsShader.setCodeFromFile("shaders/normals.fs.glsl");

    GLprogram renderProgram;
    renderProgram
        .addShader(vsShader)
        .addShader(fsShader)
        .link();
    renderProgram.use();

    GLuint singularBoidVBO;
    {
        glGenBuffers(1, &singularBoidVBO);
        glBindBuffer(GL_ARRAY_BUFFER, singularBoidVBO);
        GLfloat vertices[] = {
            0.3f, 0.0f, 0.0f, 0.3162, 0.0, 0.9487,
            -0.3, -0.2, 0.2, 0.3162, 0.0, 0.9487,
            -0.3, 0.2, 0.2, 0.3162, 0.0, 0.9487,

            -0.3, -0.2, -0.2, 0.3162, 0.0, -0.9487,
            0.3f, 0.0f, 0.0f, 0.3162, 0.0, -0.9487,
            -0.3, 0.2, -0.2, 0.3162, 0.0, -0.9487,

            -0.3, -0.2, 0.2, 0.3162, -0.9487, 0.0,
            0.3f, 0.0f, 0.0f, 0.3162, -0.9487, 0.0,
            -0.3, -0.2, -0.2, 0.3162, -0.9487, 0.0,

            -0.3, 0.2, -0.2, 0.3162, 0.9487, 0.0,
            0.3f, 0.0f, 0.0f, 0.3162, 0.9487, 0.0,
            -0.3, 0.2, 0.2, 0.3162, 0.9487, 0.0,

            -0.3, 0.2, -0.2, -1.0, 0.0, 0.0,
            -0.3, 0.2, 0.2, -1.0, 0.0, 0.0,
            -0.3, -0.2, 0.2, -1.0, 0.0, 0.0,

            -0.3, 0.2, -0.2, -1.0, 0.0, 0.0,
            -0.3, -0.2, 0.2, -1.0, 0.0, 0.0,
            -0.3, -0.2, -0.2, -1.0, 0.0, 0.0,
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    std::vector<glm::vec3> drawData(2*N);
    GLuint boidsDisplacementVBO;
    glGenBuffers(1, &boidsDisplacementVBO);

    GLuint boidsVAO;
    glGenVertexArrays(1, &boidsVAO);
    glBindVertexArray(boidsVAO);

    glBindBuffer(GL_ARRAY_BUFFER, singularBoidVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT,
        GL_FALSE, 6*sizeof(GLfloat),
        (void*)(3*sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, boidsDisplacementVBO);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
    glVertexAttribDivisor(2,1);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    glVertexAttribDivisor(3,1);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), 1.0f, 0.1f, 100.f);

    std::vector<Boid> crowd(N);
    for (Boid& boid : crowd) {
        glm::vec3 point = rng::pointInSphere(1.0f); 
        boid = Boid{
            .position = 5.0f*point,
            .direction = point,
            .speed = 0.015,
            .detectionRadius = 3.0f,
            .dodgeRadius = 2.0f,
        };
    }

    glBindVertexArray(boidsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boidsDisplacementVBO);
    while (!glfwWindowShouldClose(window.handle)) {
        glfwPollEvents();
        processInput(window.handle);   

        glm::mat4 ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        for (Boid& boid : crowd) {
            boid.update(crowd); 
            if (boid.position.x < -5) boid.position.x =  4.99;
            if (boid.position.x >  5) boid.position.x = -4.99;
            if (boid.position.y < -5) boid.position.y =  4.99;
            if (boid.position.y >  5) boid.position.y = -4.99;
            if (boid.position.z < -5) boid.position.z =  4.99;
            if (boid.position.z >  5) boid.position.z = -4.99;
        }

        for (size_t i = 0; i < N; ++i) {
            drawData[2*i] = crowd[i].position;
            drawData[2*i+1] = crowd[i].direction;
        }
        glBufferData(GL_ARRAY_BUFFER, drawData.size()*sizeof(glm::vec3), drawData.data(), GL_DYNAMIC_DRAW);
//            glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), boid.position);
            glm::mat4 MVMatrix = ViewMatrix;// * ModelMatrix;
            glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glm::mat4 VP = ProjMatrix * ViewMatrix;

            GLint locMVMatrix = glGetUniformLocation(renderProgram.getID(), "uMVMatrix");
            GLint locNormalMatrix = glGetUniformLocation(renderProgram.getID(), "uNormalMatrix");
            GLint locMVPMatrix = glGetUniformLocation(renderProgram.getID(), "uMVPMatrix");

            glUniformMatrix4fv(locMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(locNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniformMatrix4fv(locMVPMatrix, 1, GL_FALSE, glm::value_ptr(VP));


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 18, N);

        glfwSwapBuffers(window.handle);
    }

    glDeleteBuffers(1, &boidsDisplacementVBO);
    glDeleteBuffers(1, &singularBoidVBO);
    glDeleteVertexArrays(1, &boidsVAO);

    return 0;
}
