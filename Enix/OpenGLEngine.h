#pragma once
#include "Engine.h"

#include <GLFW/glfw3.h>
#include "imgui.h"

namespace Enix
{
    class OpenGLEngine : public Engine
    {
    private:
        ImVec4 clear_color_{0.45f, 0.55f, 0.60f, 1.00f};
        bool show_demo_window_{true};
        bool show_another_window_{true};
        GLFWwindow* window_ = nullptr;
        ImGuiIO io_;
    public:
        void drawUI() override;
        void tick(double deltaTime) override;
        int run() override;
    };
}
