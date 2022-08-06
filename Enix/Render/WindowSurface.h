//
// Created by Energy on 2022/08/06.
//

#pragma once


#include <vulkan/vulkan_core.h>
#include "GLFW/glfw3.h"

class WindowSurface {


    VkSurfaceKHR _surface{VK_NULL_HANDLE};
    VkInstance &_instance;
    GLFWwindow &_window;


public:

    WindowSurface(VkInstance &instance, GLFWwindow &window);

    virtual ~WindowSurface();

    VkSurfaceKHR &surface() {
        return _surface;
    }
};
