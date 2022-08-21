#include <iostream>
#include <memory>
#include <cstdlib>

#include "Engine.h"
#include "OpenGLEngine.h"
#include "VulkanEngine.h"
#include "VulkanEngineImgui.h"

#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

int main(int argc, char* argv[])
{
    // auto vkEngine = std::unique_ptr<Enix::Engine>((Enix::Engine*)new Enix::VulkanEngine{});
    // auto vkEngine = std::unique_ptr<Enix::Engine>(std::make_unique<Enix::VulkanEngine>());
    // const int vkStatus = vkEngine->run();
    // auto glEngine = std::unique_ptr<Enix::Engine>(std::make_unique<Enix::OpenGLEngine>());
    // const int glStatus = glEngine->run();


    spdlog::init_thread_pool(8192, 1);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto enixEngineLog =
            std::make_shared<spdlog::async_logger>("EnixEngine", spdlog::sinks_init_list{console_sink},
                                                   spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    enixEngineLog->set_level(spdlog::level::trace);
//        enixEngineLog->flush_on(spdlog::level::debug);
    spdlog::set_default_logger(enixEngineLog);

    try
    {
        spdlog::debug("Starting engine");

        Enix::VulkanEngine vkEngine;
        vkEngine.run();
        // manual clean up of vulkan engine
        vkEngine.cleanUp();

        // for testing purposes
        // Enix::VulkanEngineImgui vulkanEngineImgui;
        // vulkanEngineImgui.run();
    }
    catch (const std::exception& e)
    {
        spdlog::critical(e.what());
        return EXIT_FAILURE;
    }
    spdlog::debug("Exit");
    return EXIT_SUCCESS;
}
