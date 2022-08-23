#pragma once

#include <vector>
#include <unordered_map>
#include <optional>
#include <string>
#include <chrono>
#include <memory>

#include "Engine.h"
#include "Render/Renderer.h"
#include "UI/AppUI.h"
#include "Core/Scene.h"


namespace Enix {

    class VulkanEngine : public Engine {
    private:
        std::string _workspaceRoot = "../";

        // Vulkan
        Window _window;
        InputSystem _inputSystem;
        Renderer _renderer;

        bool _cleanedUp = false;
        std::chrono::steady_clock::time_point _lastTickTimePoint;

        double _deltaTime;
        std::chrono::time_point<std::chrono::steady_clock> _engineStartTimePoint;
        double _timeSinceEngineStart;

        std::shared_ptr<Scene> _scene = nullptr;
        AppUI _appUI;

        static VulkanEngine* _instance; // singleton
    public:
        VulkanEngine();

        ~VulkanEngine() override;

        static VulkanEngine& getInstance()
        {
            return *_instance;
        }
        void drawUI() override;

        void tick(double deltaTime) override;

        int init() override;

        int cleanUp() override;

        int run() override;

        // system
        [[nodiscard]] double timeSinceEngineStart() const;

        Window &window();

        Renderer &renderer() {
            return _renderer;
        }

        InputSystem &inputSystem() {
            return _inputSystem;
        }

        std::shared_ptr<Scene> &scene() {
            return _scene;
        }

        void loadScene();
    };

    inline VulkanEngine* VulkanEngine::_instance = nullptr;
}
