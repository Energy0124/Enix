﻿#pragma once
#include <vector>

#include "Engine.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <optional>
#include <string>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <array>

#include "imgui.h"

namespace Enix
{
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
            return attributeDescriptions;
        }
    };

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]] bool isComplete() const
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct UniformBufferObject
    {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    class VulkanEngine : public Engine
    {
    private:
        const uint32_t _maxFramesInFlight = 2;
        const std::vector<const char*> _validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        const std::vector<const char*> _deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        const std::vector<Vertex> _vertices = {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
        };
        const std::vector<uint32_t> _indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };
        VkDescriptorPool _descriptorPool;
        std::vector<VkDescriptorSet> _descriptorSets;
        VkImageView _textureImageView;

#ifdef NDEBUG
        const bool _enableValidationLayers = false;
#else
        const bool _enableValidationLayers = true;
#endif
        std::string _workspaceRoot = "../../";
        VkInstance _instance = nullptr;
        VkDebugUtilsMessengerEXT _debugMessenger;
        GLFWwindow* _window = nullptr;
        bool _cleanedUp = false;
        VkPhysicalDevice _physicalDevice;
        VkDevice _device;
        VkQueue _graphicsQueue;
        VkSurfaceKHR _surface;
        VkQueue _presentQueue;
        VkSwapchainKHR _swapChain;
        std::vector<VkImage> _swapChainImages;
        VkFormat _swapChainImageFormat;
        VkExtent2D _swapChainExtent;
        std::vector<VkImageView> _swapChainImageViews;
        VkRenderPass _renderPass;
        VkDescriptorSetLayout _descriptorSetLayout;
        VkPipelineLayout _pipelineLayout;
        VkPipeline _graphicsPipeline;
        std::vector<VkFramebuffer> _swapChainFramebuffers;
        VkCommandPool _commandPool;
        std::vector<VkCommandBuffer> _commandBuffers;
        std::vector<VkSemaphore> _imageAvailableSemaphores;
        std::vector<VkSemaphore> _renderFinishedSemaphores;
        std::vector<VkFence> _inFlightFences;
        uint32_t _currentFrame = 0;
        bool _framebufferResized = false;
        VkBuffer _vertexBuffer;
        VkDeviceMemory _vertexBufferMemory;
        VkBuffer _indexBuffer;
        VkDeviceMemory _indexBufferMemory;
        std::vector<VkBuffer> _uniformBuffers;
        std::vector<VkDeviceMemory> _uniformBuffersMemory;
        VkImage _textureImage;
        VkDeviceMemory _textureImageMemory;
        VkSampler _textureSampler;
        ImGuiIO* _imguiIo;
        VkDescriptorPool _imguiDescriptorPool;

        void initWindow();
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        std::vector<const char*> getRequiredExtensions();
        bool checkValidationLayerSupport();
        void createVulkanInstance();
        static VkResult createDebugUtilsMessengerExt(VkInstance instance,
                                                     const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                     const VkAllocationCallbacks* pAllocator,
                                                     VkDebugUtilsMessengerEXT* pDebugMessenger);
        static void destroyDebugUtilsMessengerExt(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                                  const VkAllocationCallbacks* pAllocator);
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void setupDebugMessenger();
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        bool isDeviceSuitable(VkPhysicalDevice device);
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSurface();
        void createSwapChain();
        void createImageViews();
        VkShaderModule createShaderModule(const std::vector<char>& code);
        void createGraphicsPipeline();
        void createRenderPass();
        void createFramebuffers();
        void createCommandPool();
        void createCommandBuffers();
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void createSyncObjects();
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                          VkBuffer& buffer, VkDeviceMemory&
                          bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void createVertexBuffer();
        void createIndexBuffer();
        void createDescriptorSetLayout();
        void createUniformBuffers();
        void createDescriptorPool();
        void createDescriptorSets();
        void createImage(int texWidth, int texHeight, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                         VkMemoryPropertyFlags
                         properties, VkImage& image, VkDeviceMemory& imageMemory);
        void createTextureImage();
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        VkImageView createImageView(VkImage image, VkFormat format);
        void createTextureImageView();
        void createTextureSampler();
        void initVulkan();
        static void glfwErrorCallback(int error, const char* description);
        static VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        static std::vector<char> readFile(const std::string& filename);
        void initImgui();
    public:
        VulkanEngine();
        ~VulkanEngine() override;
        void drawUI() override;
        void cleanupSwapChain();
        void recreateSwapChain();
        void updateUniformBuffer(uint32_t currentImage);
        void drawFrame();
        void tick() override;
        int init() override;
        int cleanUp() override;
        int run() override;
    };
}
