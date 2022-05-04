#include <memory>

#include "Engine.h"
#include "OpenGLEngine.h"
#include "VulkanEngine.h"

int main(int argc, char* argv[])
{
    auto vkEngine = std::unique_ptr<Enix::Engine>(std::make_unique<Enix::VulkanEngine>());
    const int vkStatus = vkEngine->run();
    auto glEngine = std::unique_ptr<Enix::Engine>(std::make_unique<Enix::OpenGLEngine>());
    const int glStatus = glEngine->run();
    return vkStatus;
}
