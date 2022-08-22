#pragma once

#include <array>

#include <vulkan/vulkan.h>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
namespace Enix
{
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec3 normal;
        glm::vec2 texCoord;


        constexpr friend bool operator==(const Vertex& lhs, const Vertex& rhs)
        {
            return lhs.pos == rhs.pos
                && lhs.color == rhs.color
                && lhs.texCoord == rhs.texCoord
                && lhs.normal == rhs.normal;
        }

        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(Vertex, normal);

            attributeDescriptions[3].binding = 0;
            attributeDescriptions[3].location = 3;
            attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[3].offset = offsetof(Vertex, texCoord);

            return attributeDescriptions;
        }
    };
}

namespace std
{
    template <>
    struct hash<Enix::Vertex>
    {
        size_t operator()(Enix::Vertex const& vertex) const noexcept
        {
            return ((hash<glm::vec3>()(vertex.pos) ^
                (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}