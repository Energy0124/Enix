#include "Model.h"

#include <stdexcept>

namespace Enix
{
    void Model::createVertexBuffers()
    {
        VkDeviceSize bufferSize = sizeof(_vertices[0]) * _vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        _device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                             stagingBufferMemory);

        void* data;
        vkMapMemory(_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, _vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(_device.device(), stagingBufferMemory);

        _device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer,
                             _vertexBufferMemory);

        _device.copyBuffer(stagingBuffer, _vertexBuffer, bufferSize);

        vkDestroyBuffer(_device.device(), stagingBuffer, nullptr);
        vkFreeMemory(_device.device(), stagingBufferMemory, nullptr);
    }

    void Model::createIndexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(_indices[0]) * _indices.size();
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        _device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                             stagingBufferMemory);

        void* data;
        vkMapMemory(_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, _indices.data(), (size_t)bufferSize);
        vkUnmapMemory(_device.device(), stagingBufferMemory);

        _device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer,
                             _indexBufferMemory);

        _device.copyBuffer(stagingBuffer, _indexBuffer, bufferSize);

        vkDestroyBuffer(_device.device(), stagingBuffer, nullptr);
        vkFreeMemory(_device.device(), stagingBufferMemory, nullptr);
    }

    Model::Model(const Device& device, std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices): _device(device),
        _vertices(vertices), _indices(indices)
    {
        createVertexBuffers();
        createIndexBuffer();
    }

    void Model::releaseResources() const {
        // destroy vertex buffer
        vkDestroyBuffer(_device.device(), _vertexBuffer, nullptr);
        vkFreeMemory(_device.device(), _vertexBufferMemory, nullptr);

        // destroy index buffer
        vkDestroyBuffer(_device.device(), _indexBuffer, nullptr);
        vkFreeMemory(_device.device(), _indexBufferMemory, nullptr);
    }

    Model::~Model()
    {
        // todo: fix this later, we should use raii here
        // releaseResources();
    }

    void Model::bind(VkCommandBuffer commandBuffer) const {
        VkBuffer vertexBuffers[] = {_vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }

    void Model::draw(VkCommandBuffer commandBuffer) const {
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(_indices.size()), 1, 0, 0, 0);
    }
}
