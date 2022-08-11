#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include <vector>
#include "Vertex.h"
#include "Device.h"

namespace Enix
{
    class Model
    {
        void createVertexBuffers();
        void createIndexBuffer();


        const Device& _device;
        std::vector<Vertex> _vertices;
        VkBuffer _vertexBuffer;
        VkDeviceMemory _vertexBufferMemory;
        std::vector<uint32_t> _indices;
        VkBuffer _indexBuffer;
        VkDeviceMemory _indexBufferMemory;

        void releaseResources() const;
    public:
        Model(const Device& device, std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices);

        ~Model();
        Model(const Model& other) = delete;
        Model& operator=(const Model& other) = delete;
        
        void bind(VkCommandBuffer commandBuffer) const;
        void draw(VkCommandBuffer commandBuffer) const;
    };
}
