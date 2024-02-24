#pragma once

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


GLFWwindow *basicWindowInit(int width, int height, std::string title);
