#pragma once
#include <vector>

#include "Engine.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <optional>
#include <xstring>

namespace Enix
{
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]] bool isComplete() const
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
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
        bool checkDeviceExtensionSupport(VkPhysicalDevice device) ;
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
