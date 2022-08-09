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


namespace Enix {


    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };
    struct MeshPushConstant {
        alignas(16) glm::mat4 modelMatrix{1.f};
    };

    class VulkanEngine : public Engine {
    private:
        const uint32_t _maxFramesInFlight = 2;
        const std::vector<const char *> _validationLayers{
                "VK_LAYER_KHRONOS_validation"
        };
        const std::vector<const char *> _deviceExtensions{
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
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

//        std::unique_ptr<Device> _enixDevice;
        std::unique_ptr<MeshAsset> _meshAsset;

//        VkDevice _device;
//        VkPhysicalDevice _physicalDevice;
//        VkQueue _graphicsQueue;
//        VkQueue _presentQueue;

        VkDescriptorPool _descriptorPool;
        std::vector<VkDescriptorSet> _descriptorSets;
        VkImageView _textureImageView;



        bool _cleanedUp = false;


//        VkSwapchainKHR _swapChain;
//        std::vector<VkImage> _swapChainImages;
//        VkFormat _swapChainImageFormat;
//        VkExtent2D _swapChainExtent;
//        std::vector<VkImageView> _swapChainImageViews;
//        std::vector<VkFramebuffer> _swapChainFramebuffers;
//        VkRenderPass _renderPass;
//        VkImage _depthImage;
//        VkDeviceMemory _depthImageMemory;
//        VkImageView _depthImageView;


        VkDescriptorSetLayout _descriptorSetLayout;
        VkPipelineLayout _pipelineLayout;
        VkPipeline _graphicsPipeline;
        std::vector<VkCommandBuffer> _commandBuffers;
        std::vector<VkSemaphore> _imageAvailableSemaphores;
        std::vector<VkSemaphore> _renderFinishedSemaphores;
        std::vector<VkFence> _inFlightFences;
        uint32_t _currentFrame = 0;
//        bool _framebufferResized = false;
        std::vector<VkBuffer> _uniformBuffers;
        std::vector<VkDeviceMemory> _uniformBuffersMemory;
        VkImage _textureImage;
        VkDeviceMemory _textureImageMemory;
        VkSampler _textureSampler;
        ImGuiIO *_imguiIo;
        VkDescriptorPool _imguiDescriptorPool;

        std::chrono::steady_clock::time_point _lastTickTimePoint;

        double _deltaTime;
        std::chrono::time_point <std::chrono::steady_clock> _engineStartTimePoint;
        double _timeSinceEngineStart;


        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);


        VkShaderModule createShaderModule(const std::vector<char> &code);

        void createGraphicsPipeline();

        void createCommandBuffers();

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        void createSyncObjects();

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                          VkBuffer &buffer, VkDeviceMemory &
        bufferMemory);

        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

        void createDescriptorSetLayout();

        void createUniformBuffers();

        void createDescriptorPool();

        void createDescriptorSets();

        void createTextureImage();

        void createTextureImageView();

        void createTextureSampler();

        void initVulkan();

        static std::vector<char> readFile(const std::string &filename);

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
