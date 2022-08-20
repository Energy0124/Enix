//
// Created by Energy on 2022/08/13.
//

#pragma once

// forward declarations
namespace Enix {
    class VulkanEngine;
}

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

#include <imgui.h>

#include "Render/Vertex.h"
#include "Render/Model.h"
#include "Render/Window.h"
#include "Asset/MeshAsset.h"
#include "Render/Instance.h"
#include "Render/WindowSurface.h"
#include "Render/SwapChain.h"
#include "Render/GraphicsPipeline.h"
#include "Core/Actor.h"
#include "Core/Camera.h"
#include "Core/MeshActor.h"


namespace Enix {
    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    class Renderer {

        const uint32_t _maxFramesInFlight = 2;

#ifdef NDEBUG
        const bool _enableValidationLayers = false;
#else
        const bool _enableValidationLayers = true;
#endif

        std::string _workspaceRoot = "../";
        std::string _modelPath = "Models/viking_room.obj";
        std::string _model2Path = "Models/untitled.obj";
        std::string _texturePath = "Textures/viking_room.png";
        std::string _texture2Path = "Textures/untitled.png";


        // Vulkan
        Window &_window;
        Instance _instance;
        WindowSurface _surface;
        Device _device;
        RenderPass _renderPass;
        SwapChain _swapChain;
        GraphicsPipeline _graphicsPipeline;

        std::shared_ptr<MeshAsset> _meshAsset;
        std::shared_ptr<MeshAsset> _meshAsset2;
        std::shared_ptr<MeshActor> _actor;

        std::shared_ptr<Camera> _camera;
        std::vector<std::shared_ptr<MeshActor>> _meshActors{};
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

        VulkanEngine &_engine;

        void createCommandBuffers();

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        void createSyncObjects();

        void createUniformBuffers();

        void createDescriptorPool();

        void createDescriptorSets();

        void initVulkan();

        void initImgui();

        void updateUniformBuffer(uint32_t currentImage);

        void cleanUp();

        void drawFrame();

        void drawUI();

        void updateCamera();

        void createRenderObjects();

    public:
        Renderer(VulkanEngine &engine);

        ~Renderer();

        void init();

        void draw();

        std::shared_ptr<Camera> &camera() {
            return _camera;
        }

        std::vector<std::shared_ptr<MeshActor>> &meshActors() {
            return _meshActors;
        }

    };

} // Enix
