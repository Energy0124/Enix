#pragma once
#include <imgui.h>
#include <GLFW/glfw3.h>

#include "imgui_impl_vulkan.h"

namespace Enix
{
    class Engine
    {

    private:
        virtual int init() = 0;
        virtual int cleanUp() = 0;

    public:
        Engine(const Engine& other) = delete;
        Engine(Engine&& other) noexcept = delete;
        Engine& operator=(const Engine& other) = delete;
        Engine& operator=(Engine&& other) noexcept = delete;

        Engine() = default;
        virtual ~Engine() = default;
        
        virtual void drawUI() = 0;

        virtual void tick() = 0;
        
        virtual int run() = 0;
        
       
    };
}
