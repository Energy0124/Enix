#pragma once
#include <imgui.h>
#include <GLFW/glfw3.h>

#include "imgui_impl_vulkan.h"

namespace Enix
{
    class Engine
    {
    private:
        ImVec4 clear_color_{0.45f, 0.55f, 0.60f, 1.00f};
        bool show_demo_window{true};
        bool show_another_window{true};
    public:
        void drawUI();

        void tick(GLFWwindow* window, ImGui_ImplVulkanH_Window* wd, ImGuiIO& io);

        int run();
    };
}
