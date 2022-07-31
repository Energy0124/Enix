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

        // implicitly cast to VkDevice, for convenience, consider remove it later
        operator VkDevice() const { return _device; }

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                          VkBuffer& buffer,
                          VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        [[nodiscard]] VkDevice& device() const { return _device; }
    };
}
