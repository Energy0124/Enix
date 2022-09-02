//
// Created by Energy on 2022/08/11.
//

#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>

#include <vulkan/vulkan_core.h>
#include "Device.h"
#include "SwapChain.h"

namespace Enix {
    struct MeshPushConstant {
        alignas(16) glm::mat4 modelMatrix{1.f};
        alignas(16) glm::mat4 normalMatrix{1.f};
    };

    class GraphicsPipeline {
        std::vector<VkDescriptorSetLayout> _descriptorSetLayouts;
        VkPipeline _graphicsPipeline;
        VkPipelineLayout _pipelineLayout;
        VkShaderModule _vertShaderModule;
        VkShaderModule _fragShaderModule;

        Device &_device;
        SwapChain &_swapChain;
        RenderPass &_renderPass;

        const std::string _workspaceRoot;

    private:
        void createGraphicsPipeline();

        VkShaderModule createShaderModule(const std::vector<char> &code);

        void createDescriptorSetLayout();

    public:
        GraphicsPipeline(std::string workspaceRoot, Device &device, SwapChain &swapChain,
                         RenderPass &renderPass);

        ~GraphicsPipeline();

        GraphicsPipeline(const GraphicsPipeline &) = delete;

        GraphicsPipeline &operator=(const GraphicsPipeline &) = delete;

        [[nodiscard]] std::vector<VkDescriptorSetLayout> &descriptorSetLayouts() {
            return _descriptorSetLayouts;
        }

        [[nodiscard]] VkPipeline graphicsPipeline() const {
            return _graphicsPipeline;
        }

        [[nodiscard]] VkPipelineLayout pipelineLayout() const {
            return _pipelineLayout;
        }


    };

} // Enix
