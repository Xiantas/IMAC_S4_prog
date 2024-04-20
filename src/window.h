#pragma once

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


auto basicWindowInit(int width, int height, std::string title) -> GLFWwindow*;

// A lightweight wrapper around the GLFWwindow type to provide a destructor
// This class also takes care of glfw context since we only plan on having one window
class Window {
public:
    GLFWwindow *handle;

    Window() = delete;
    Window(int width, int height, std::string title);
    // Make it so that the type can't be passed around
    // we use the handle instead
    // (it is truly a minimal wrapped)
    Window(Window const &window) = delete;
    Window(Window &&window) = delete;
    ~Window();
};
