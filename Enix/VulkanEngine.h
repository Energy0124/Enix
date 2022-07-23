#pragma once
#include <vector>

#include "Engine.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <optional>
#include <xstring>
#include <glm/glm.hpp>
#include <array>

namespace Enix
{
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);
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

    class VulkanEngine : public Engine
    {
    private:
        const int maxFramesInFlight_ = 2;
        const std::vector<const char*> validationLayers_ = {
            "VK_LAYER_KHRONOS_validation"
        };
        const std::vector<const char*> deviceExtensions_ = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        const std::vector<Vertex> vertices_ = {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
#ifdef NDEBUG
        const bool enableValidationLayers_ = false;
#else
        const bool enableValidationLayers_ = true;
#endif
        VkInstance instance_ = nullptr;
        VkDebugUtilsMessengerEXT debugMessenger_;
        GLFWwindow* window_ = nullptr;
        bool cleanedUp_ = false;
        VkPhysicalDevice physicalDevice_;
        VkDevice device_;
        VkQueue graphicsQueue_;
        VkSurfaceKHR surface_;
        VkQueue presentQueue_;
        VkSwapchainKHR swapChain_;
        std::vector<VkImage> swapChainImages_;
        VkFormat swapChainImageFormat_;
        VkExtent2D swapChainExtent_;
        std::vector<VkImageView> swapChainImageViews_;
        VkRenderPass renderPass_;
        VkPipelineLayout pipelineLayout_;
        VkPipeline graphicsPipeline_;
        std::vector<VkFramebuffer> swapChainFramebuffers_;
        VkCommandPool commandPool_;
        std::vector<VkCommandBuffer> commandBuffers_;
        std::vector<VkSemaphore> imageAvailableSemaphores_;
        std::vector<VkSemaphore> renderFinishedSemaphores_;
        std::vector<VkFence> inFlightFences_;
        uint32_t currentFrame_ = 0;
        bool framebufferResized_ = false;
        VkBuffer vertexBuffer_;
        VkDeviceMemory vertexBufferMemory_;

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
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory&
                          bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void createVertexBuffer();
        void initVulkan();
        static void glfwErrorCallback(int error, const char* description);
        static VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        static std::vector<char> readFile(const std::string& filename);
    public:
        VulkanEngine();
        ~VulkanEngine() override;
        void drawUI() override;
        void cleanupSwapChain();
        void recreateSwapChain();
        void drawFrame();
        void tick() override;
        int init() override;
        int cleanUp() override;
        int run() override;
    };
}
