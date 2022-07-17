#pragma once
#include <vector>

#include "Engine.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace Enix
{
    class VulkanEngine : public Engine
    {
    private:
        const std::vector<const char*> validationLayers_ = {
            "VK_LAYER_KHRONOS_validation"
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

        void initWindow();
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
        void initVulkan();
        static void glfwErrorCallback(int error, const char* description);
        static VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    public:
        VulkanEngine();
        ~VulkanEngine() override;
        void drawUI() override;
        void tick() override;
        int init() override;
        int cleanUp() override;
        int run() override;
    };
}
