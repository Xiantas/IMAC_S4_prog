#include <cstdlib>
#include <iostream>
#include <span>
#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#define DOCTEST_CONFIG_IMPLEMENT
#include "boid.h"
#include "doctest/doctest.h"
#include "file_utils.h"
#include "openGL/arrays.h" 
#include "openGL/program.h"
#include "object3D.h"
#include "renderer.h"
#include "random_utils.h"
#include "window.h"


constexpr size_t N = 500;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void processInput(GLFWwindow* window, Renderer &renderer) {
    const float cameraSpeed = 0.05f; 
    glm::vec3 up(0.0, 1.0, 0.0);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        renderer.camera.pos += cameraSpeed * renderer.camera.dir;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        renderer.camera.pos -= cameraSpeed * renderer.camera.dir;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        renderer.camera.pos -= glm::normalize(glm::cross(renderer.camera.dir, up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        renderer.camera.pos += glm::normalize(glm::cross(renderer.camera.dir, up)) * cameraSpeed;
}

int main() {
    Window window(1000, 1000, std::string("Machin"));


    Renderer renderer(Camera(
        glm::vec3(3.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        70.0f
    ));

    Object3D turtle(VaoType::Classic);
    turtle
        .loadObj("assets/models/Turtle.obj")
        .loadTexture("assets/textures/turtle_texture.png")
        .setProgram(renderer, "shaders/singleObject.vs.glsl", "shaders/normals.fs.glsl");
    Object3D aqua(VaoType::Classic);
    aqua
        .loadObj("assets/models/world.obj")
        .loadTexture("assets/textures/texture.png")
        .setProgram(renderer, "shaders/singleObject.vs.glsl", "shaders/normals.fs.glsl");
    Object3D pwasson(VaoType::Classic);
    pwasson
        .loadObj("assets/models/ClownFish.obj")
        .loadTexture("assets/textures/texture_clownfish.png")
        .setProgram(renderer, "shaders/singleObject.vs.glsl", "shaders/normals.fs.glsl");
        

    /*
    std::vector<glm::vec3> drawData(2*N);
    */

    glEnable(GL_DEPTH_TEST);

    /*
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
    */

    while (!glfwWindowShouldClose(window.handle)) {
        glfwPollEvents();
        processInput(window.handle, renderer);   

        
        /*
        for (Boid& boid : crowd) {
            boid.update(crowd); 
            if (boid.position.x < -5) boid.position.x =  4.99;
            if (boid.position.x >  5) boid.position.x = -4.99;
            if (boid.position.y < -5) boid.position.y =  4.99;
            if (boid.position.y >  5) boid.position.y = -4.99;
            if (boid.position.z < -5) boid.position.z =  4.99;
            if (boid.position.z >  5) boid.position.z = -4.99;
        }
        */

        /*
        for (size_t i = 0; i < N; ++i) {
            drawData[2*i] = crowd[i].position;
            drawData[2*i+1] = crowd[i].direction;
        }
        glBufferData(GL_ARRAY_BUFFER, drawData.size()*sizeof(glm::vec3), drawData.data(), GL_DYNAMIC_DRAW);
            */


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        aqua.render(renderer);
        pwasson.render(renderer);
        turtle.render(renderer);

        glfwSwapBuffers(window.handle);
    }

    return 0;
}
