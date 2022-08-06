#include "Window.h"

#include <iostream>
#include <stdexcept>

#include "GLFW/glfw3.h"

namespace Enix
{
    Window::Window()
    {
        initWindow();
    }

    Window::~Window()
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }


    void Window::glfwErrorCallback(int error, const char* description)
    {
        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    }

    void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->_framebufferResized = true;
    }

    void Window::initWindow()
    {
        // Setup GLFW window
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        _window = glfwCreateWindow(1280, 720, "Enix Engine", nullptr, nullptr);
        glfwSetWindowUserPointer(_window, this);
        glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
    }
}
