//
// Created by Energy on 2022/08/05.
//

#pragma once

#include <vulkan/vulkan.h>

class Instance {
private:
    const std::vector<const char *> _validationLayers{
            "VK_LAYER_KHRONOS_validation"
    };

    VkInstance _instance{VK_NULL_HANDLE};
    bool _enableValidationLayers;
    VkDebugUtilsMessengerEXT _debugMessenger{VK_NULL_HANDLE};

    void setupDebugMessenger();

    bool checkValidationLayerSupport();

    [[nodiscard]]
    std::vector<const char *> getRequiredExtensions() const;

    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    static VkBool32
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);


    static VkResult
    createDebugUtilsMessengerExt(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                 const VkAllocationCallbacks *pAllocator,
                                 VkDebugUtilsMessengerEXT *pDebugMessenger);

    void createVkInstance();

    static void destroyDebugUtilsMessengerExt(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                              const VkAllocationCallbacks *pAllocator);

public:
    explicit Instance(bool enableValidationLayers);

    ~Instance();

    [[nodiscard]]
    VkInstance& instance() {
        return _instance;
    }


};
