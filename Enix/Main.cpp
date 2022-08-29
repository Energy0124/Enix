#include <iostream>
#include <memory>
#include <cstdlib>

#include "Engine.h"
#include "VulkanEngine.h"

#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

int main(int argc, char* argv[])
{
    // setup spdlog
    spdlog::init_thread_pool(8192, 1);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto enixEngineLog =
            std::make_shared<spdlog::async_logger>("EnixEngine", spdlog::sinks_init_list{console_sink},
                                                   spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    enixEngineLog->set_level(spdlog::level::trace);
    enixEngineLog->flush_on(spdlog::level::debug);
    spdlog::set_default_logger(enixEngineLog);

    try
    {
        spdlog::debug("Starting engine");

        {
            Enix::VulkanEngine vkEngine;
            vkEngine.run();
        } // clean up engine
    }
    catch (const std::exception& e)
    {
        spdlog::critical(e.what());
        return EXIT_FAILURE;
    }
    spdlog::debug("Exit");
    return EXIT_SUCCESS;
}
