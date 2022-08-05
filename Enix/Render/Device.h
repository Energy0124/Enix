#pragma once
#include <vulkan/vulkan.h>

namespace Enix
{
    class Device
    {
        VkPhysicalDevice& _physicalDevice;
        VkQueue& _graphicsQueue;
        VkCommandPool& _commandPool;
        VkDevice& _device;
    public:
        Device(VkDevice& device, VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue, VkCommandPool& commandPool);
        ~Device();

        [[nodiscard]] uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                          VkBuffer& buffer,
                          VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
        [[nodiscard]] VkCommandBuffer beginSingleTimeCommands() const;
        void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;
        [[nodiscard]] VkDevice device() const { return _device; }

        void
        createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,
                     VkDeviceMemory &bufferMemory) const;
    };
}
