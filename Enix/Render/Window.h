#pragma once
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>
#include <functional>

namespace Enix
{
    class Window
    {
    public:
        typedef std::function<void(int, int, int, int)> onKeyFunc;
    private:
        GLFWwindow* _window = nullptr;
        bool _framebufferResized = false;
        std::vector<onKeyFunc> _onKeyFunc;
    public:
        Window();
        ~Window();

        [[nodiscard]] GLFWwindow& window() const
        {
            return *_window;
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
        void initWindow();
        void waitForFocus();

        static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

        void onKey(int key, int scanCode, int action, int mods);
        void registerOnKeyFunc(const onKeyFunc& func) { _onKeyFunc.emplace_back(func); }

    };
}
