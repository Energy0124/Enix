//
// Created by Energy on 2022/08/06.
//

#include <stdexcept>
#include "WindowSurface.h"

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"


WindowSurface::WindowSurface(VkInstance &instance, GLFWwindow& window) : _instance(instance), _window(window) {

    if (!glfwVulkanSupported()) {
        throw std::runtime_error("GLFW: Vulkan not supported");
    }

    if (glfwCreateWindowSurface(_instance, &_window, nullptr, &_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}

WindowSurface::~WindowSurface() {
    vkDestroySurfaceKHR(_instance, _surface, nullptr);
}
