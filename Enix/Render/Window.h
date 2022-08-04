#pragma once
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.h"

namespace Enix
{
    class Window
    {
    private:
        GLFWwindow* _window = nullptr;
        bool _framebufferResized = false;
    public:
        Window();
        ~Window();

        [[nodiscard]] operator GLFWwindow*() const
        {
            return _window;
        }

        [[nodiscard]] bool framebufferResized() const
        {
            return _framebufferResized;
        }

        void setFramebufferResized(bool framebufferResized)
        {
            _framebufferResized = framebufferResized;
        }

        static void glfwErrorCallback(int error, const char* description);
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        void createSurface(VkInstance instance, VkSurfaceKHR& surface);
        void initWindow();
    };
}
