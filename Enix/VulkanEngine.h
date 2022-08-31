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
#include "Core/ThreadPool.h"


namespace Enix {

    class VulkanEngine : public Engine {
    private:
        std::string _workspaceRoot = "../";

        Window _window;
        InputSystem _inputSystem;
        Renderer _renderer;
        std::shared_ptr<Scene> _scene = nullptr;
        AppUI _appUI;
        ThreadPool _threadPool;

        bool _cleanedUp = false;
        std::chrono::steady_clock::time_point _lastTickTimePoint;
        double _deltaTime = 0;
        std::chrono::time_point<std::chrono::steady_clock> _engineStartTimePoint{};
        double _timeSinceEngineStart = 0;



        static VulkanEngine *_instance; // singleton
    public:
        VulkanEngine();

        ~VulkanEngine() override;

        static VulkanEngine &getInstance() {
            return *_instance;
        }

        void drawUI() override;

        void tick(double deltaTime) override;

        int init();

        int cleanUp();

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

        ThreadPool &threadPool() {
            return _threadPool;
        }

        [[nodiscard]] std::shared_ptr<Scene> &scene() {
            return _scene;
        }

        void loadScene();
    };

    inline VulkanEngine *VulkanEngine::_instance = nullptr;
}
