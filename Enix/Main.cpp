#include <iostream>
#include <memory>


#include "Engine.h"
#include "OpenGLEngine.h"
#include "VulkanEngine.h"

int main(int argc, char* argv[])
{
    // auto vkEngine = std::unique_ptr<Enix::Engine>((Enix::Engine*)new Enix::VulkanEngine{});
    // auto vkEngine = std::unique_ptr<Enix::Engine>(std::make_unique<Enix::VulkanEngine>());
    // const int vkStatus = vkEngine->run();
    // auto glEngine = std::unique_ptr<Enix::Engine>(std::make_unique<Enix::OpenGLEngine>());
    // const int glStatus = glEngine->run();

    Enix::VulkanEngine vkEngine;
    const int vkStatus = vkEngine.run();
    return vkStatus;
}




