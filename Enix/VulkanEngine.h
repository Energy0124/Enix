#pragma once

#include <vector>
#include <unordered_map>
#include <optional>
#include <string>
#include <chrono>
#include <memory>

#include "Engine.h"
#include "Render/Renderer.h"


namespace Enix {

    class VulkanEngine : public Engine {
    private:

        // Vulkan
        Window _window;
        Renderer _renderer;

        bool _cleanedUp = false;
        std::chrono::steady_clock::time_point _lastTickTimePoint;

        double _deltaTime;
        std::chrono::time_point <std::chrono::steady_clock> _engineStartTimePoint;
        double _timeSinceEngineStart;


    public:
        VulkanEngine();

        ~VulkanEngine() override;

        void drawUI() override;

        void tick(double deltaTime) override;

        int init() override;

        int cleanUp() override;

        int run() override;

        // system
        [[nodiscard]] double timeSinceEngineStart() const;

        Window &window();
    };
}
