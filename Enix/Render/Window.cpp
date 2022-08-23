#include "Window.h"

#include <iostream>
#include <stdexcept>

#include "GLFW/glfw3.h"

namespace Enix {
    Window::Window() {
        initWindow();
    }

    Window::~Window() {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }


    void Window::glfwErrorCallback(int error, const char *description) {
        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    }

    void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto pWindow = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        pWindow->_framebufferResized = true;
    }

    void Window::initWindow() {
        // Setup GLFW window
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        _window = glfwCreateWindow(1280, 720, "Enix Engine", nullptr, nullptr);
        glfwSetWindowUserPointer(_window, this);
        glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);

        glfwSetKeyCallback(_window, keyCallback);
    }
    void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto pWindow = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        if (pWindow)
        {
            pWindow->onKey(key, scancode, action, mods);
        }
    }

    void Window::waitForFocus() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(_window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(_window, &width, &height);
            glfwWaitEvents();
        }
    }

    void Window::onKey(int key, int scanCode, int action, int mods) {
        for (auto& func : _onKeyFunc)
            func(key, scanCode, action, mods);
    }
}
