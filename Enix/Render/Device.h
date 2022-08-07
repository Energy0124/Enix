﻿#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>


namespace Enix {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]] bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };


    class Device {
        const std::vector<const char *> _deviceExtensions{
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        const std::vector<const char *> _validationLayers{
                "VK_LAYER_KHRONOS_validation"
        };
        bool _enableValidationLayers;


        VkPhysicalDevice _physicalDevice{VK_NULL_HANDLE};
        VkDevice _device{VK_NULL_HANDLE};
        VkQueue _graphicsQueue{VK_NULL_HANDLE};
        VkQueue _presentQueue{VK_NULL_HANDLE};
        VkCommandPool _commandPool{VK_NULL_HANDLE};

        VkInstance &_instance;
        VkSurfaceKHR &_surface;


        void pickPhysicalDevice();

        void createLogicalDevice();

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        bool isDeviceSuitable(VkPhysicalDevice device);

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        void createCommandPool();
    public:
        explicit Device(bool enableValidationLayers, VkInstance &instance, VkSurfaceKHR &surface);

        ~Device();

        [[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;

        [[nodiscard]] VkCommandBuffer beginSingleTimeCommands() const;

        void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;


        void
        createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,
                     VkDeviceMemory &bufferMemory) const;


        [[nodiscard]] VkDevice device() const { return _device; }
        // for convenience
        [[nodiscard]] operator VkDevice() const { return _device; }
        [[nodiscard]] VkPhysicalDevice physicalDevice() const { return _physicalDevice; }
        [[nodiscard]] VkQueue graphicsQueue() const { return _graphicsQueue; }
        [[nodiscard]] VkQueue presentQueue() const { return _presentQueue; }
        [[nodiscard]] VkCommandPool commandPool() const { return _commandPool; }


    };
}
