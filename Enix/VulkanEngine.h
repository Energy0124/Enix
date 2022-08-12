#pragma once

#include <vector>
#include <unordered_map>
#include <optional>
#include <string>
#include <chrono>
#include <memory>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


#include "Engine.h"
#include "imgui.h"
#include "Render/Vertex.h"
#include "Render/Model.h"
#include "Render/Window.h"
#include "Asset/MeshAsset.h"
#include "Render/Instance.h"
#include "Render/WindowSurface.h"
#include "Render/SwapChain.h"
#include "Render/GraphicsPipeline.h"


namespace Enix {


    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };


    class VulkanEngine : public Engine {
    private:
        const uint32_t _maxFramesInFlight = 2;

#ifdef NDEBUG
        const bool _enableValidationLayers = false;
#else
        const bool _enableValidationLayers = true;
#endif

        std::string _workspaceRoot = "../";
        std::string _modelPath = "Models/viking_room.obj";
        std::string _texturePath = "Textures/viking_room.png";

        // Vulkan
        Window _window;
        Instance _instance;
        WindowSurface _surface;
        Device _device;
        RenderPass _renderPass;
        SwapChain _swapChain;
        GraphicsPipeline _graphicsPipeline;

        std::unique_ptr<MeshAsset> _meshAsset;

        VkDescriptorPool _descriptorPool;
        VkDescriptorPool _imguiDescriptorPool;
        std::vector<VkDescriptorSet> _descriptorSets;

        std::vector<VkCommandBuffer> _commandBuffers;

        std::vector<VkBuffer> _uniformBuffers;
        std::vector<VkDeviceMemory> _uniformBuffersMemory;

        std::vector<VkSemaphore> _imageAvailableSemaphores;
        std::vector<VkSemaphore> _renderFinishedSemaphores;
        std::vector<VkFence> _inFlightFences;

        ImGuiIO *_imguiIo;

        uint32_t _currentFrame = 0;

        bool _cleanedUp = false;
        std::chrono::steady_clock::time_point _lastTickTimePoint;

        double _deltaTime;
        std::chrono::time_point <std::chrono::steady_clock> _engineStartTimePoint;
        double _timeSinceEngineStart;

        void createCommandBuffers();

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        void createSyncObjects();

        void createUniformBuffers();

        void createDescriptorPool();

        void createDescriptorSets();

        void initVulkan();

        void initImgui();

        void updateUniformBuffer(uint32_t currentImage);

        void drawFrame();

    public:
        VulkanEngine();

        ~VulkanEngine() override;

        void drawUI() override;

        void tick(double deltaTime) override;

        int init() override;

        int cleanUp() override;

        int run() override;


    };
}
