#pragma once
#include "Engine.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "imgui.h"
#include "imgui_impl_vulkan.h"

namespace Enix
{
    class VulkanEngineImgui : public Engine
    {
    private:
        ImVec4 clear_color_{0.45f, 0.55f, 0.60f, 1.00f};
        bool show_demo_window_{true};
        bool show_another_window_{true};
        ImGui_ImplVulkanH_Window* wd_ = nullptr;
        ImGuiIO io_;
        GLFWwindow* window_ = nullptr;
        int init() override;
        int cleanUp() override;
        void initWindow();
        void initVulkan();
    public:
        VulkanEngineImgui();
        ~VulkanEngineImgui() override;
        void drawUI() override;
        void tick() override;
        
        int run() override;
    };
}
