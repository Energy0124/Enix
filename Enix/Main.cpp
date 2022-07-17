#include <iostream>
#include <memory>
#include <cstdlib>

#include "Engine.h"
#include "OpenGLEngine.h"
#include "VulkanEngine.h"
#include "VulkanEngineImgui.h"

int main(int argc, char* argv[])
{
    // auto vkEngine = std::unique_ptr<Enix::Engine>((Enix::Engine*)new Enix::VulkanEngine{});
    // auto vkEngine = std::unique_ptr<Enix::Engine>(std::make_unique<Enix::VulkanEngine>());
    // const int vkStatus = vkEngine->run();
    // auto glEngine = std::unique_ptr<Enix::Engine>(std::make_unique<Enix::OpenGLEngine>());
    // const int glStatus = glEngine->run();

    try
    {
        Enix::VulkanEngine vkEngine;
        // vkEngine.init();
        vkEngine.run();
        // manual clean up of vulkan engine
        vkEngine.cleanUp();

        // for testing purposes
        // Enix::VulkanEngineImgui vulkanEngineImgui;
        // vulkanEngineImgui.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
