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

    VkInstance _instance = nullptr;
    bool _enableValidationLayers;

    bool checkValidationLayerSupport();

    [[nodiscard]] std::vector<const char *> getRequiredExtensions() const;

    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    static VkBool32
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

public:
    explicit Instance(bool enableValidationLayers);

    ~Instance();

    [[nodiscard]]  VkInstance instance() const {
        return _instance;
    }

};
