#pragma once
#include <imgui.h>
#include <GLFW/glfw3.h>

#include "imgui_impl_vulkan.h"

namespace Enix
{
    class Engine
    {
    public:
        virtual void drawUI() = 0;

        virtual void tick() = 0;

        virtual int run() = 0;
        virtual ~Engine() = default;
    };
}
