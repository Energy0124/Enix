#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <set>
#include <algorithm>
#include <chrono>

#include <spdlog/spdlog.h>

#include "VulkanEngine.h"

// for temporary debugging purposes
static auto t1 = std::chrono::high_resolution_clock::now();

void DEBUG_startTimer(const std::string &&message) {
    using std::chrono::high_resolution_clock;
    t1 = high_resolution_clock::now();
    std::cout << "start timer: " << message << std::endl;
}

void DEBUG_logTimer(const std::string &&message) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms = t2 - t1;
    std::cout << message << ": " << ms.count() << "ms" << std::endl;
}

namespace Enix {

    VulkanEngine::VulkanEngine() :
            _window(), _renderer(*this), _appUI(*this) {
        spdlog::debug("construct engine");

        init();
    }

    VulkanEngine::~VulkanEngine() {

    }

    void VulkanEngine::drawUI() {

        _appUI.onDraw();
    }

    int VulkanEngine::init() {
        spdlog::debug("init engine");

        return 0;
    }

    void VulkanEngine::tick(double deltaTime) {
        glfwPollEvents();
        _scene->tick(deltaTime);
        _renderer.draw();
    }

    int VulkanEngine::run() {
        loadScene();

        using namespace std::chrono;
        double deltaTime;
        _engineStartTimePoint = steady_clock::now();
        // Main loop
        while (!glfwWindowShouldClose(&_window.window())) {

            steady_clock::time_point tickTimePoint = steady_clock::now();
            auto timeSpan = duration_cast<duration<double >>(tickTimePoint - _lastTickTimePoint);
            deltaTime = timeSpan.count();

            _lastTickTimePoint = tickTimePoint;

//            std::cout << "deltaTime: " << deltaTime << std::endl;
            _deltaTime = deltaTime;
            _timeSinceEngineStart =
                    duration_cast<duration<double >>(tickTimePoint - _engineStartTimePoint).count();
            tick(deltaTime);
        }

//        vkDeviceWaitIdle(_device);


        return 0;
    }

    int VulkanEngine::cleanUp() {
        if (_cleanedUp) {
            return 0;
        }

        spdlog::debug("Cleaning up engine");

        _cleanedUp = true;
        return 0;
    }

    double VulkanEngine::timeSinceEngineStart() const {
        return _timeSinceEngineStart;
    }

    Window &VulkanEngine::window() {
        return _window;
    }

    void VulkanEngine::loadScene() {

        _scene = std::make_shared<Scene>();

        // todo: create a scene and pass the scene to renderer to create render object
        _renderer.createRenderObjects(_scene);
    }

}
