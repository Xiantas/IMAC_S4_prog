#include "window.h"

#include <string>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>


Window::Window(int width, int height, std::string title) 
{
    if (!glfwInit()) {
        std::cout << "Couldn't init glfw\n";
        exit(-1);
    }

#ifdef __APPLE__
    /* We need to explicitly ask for a 3.3 context on Mac */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    glfwSetErrorCallback([](int error, const char* description) {
        std::cout << "glfw error #" << error << ": " << description << "\n";
    });

    this->handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

    if (!this->handle) {
        std::cout << "Couldn't create the window\n";
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(this->handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        exit(-1);
    }
    
	glfwSetKeyCallback(this->handle, [] (GLFWwindow* window, int key, int, int action, int) {
        if (action == GLFW_PRESS) {
            if (key == GLFW_KEY_ESCAPE) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            } 
        }
    });
}

Window::~Window() {
    glfwTerminate();
}
