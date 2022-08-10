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
    };

    class GraphicsPipeline {
        VkDescriptorSetLayout _descriptorSetLayout;
        VkPipeline _graphicsPipeline;
        VkPipelineLayout _pipelineLayout;
        VkShaderModule _vertShaderModule;
        VkShaderModule _fragShaderModule;

        Device &_device;
        SwapChain &_swapChain;
        RenderPass &_renderPass;

        const std::string _workspaceRoot;

    private:
    public:
        GraphicsPipeline(std::string workspaceRoot, Device &device, SwapChain &swapChain,
                         RenderPass &renderPass);

        virtual ~GraphicsPipeline();

         [[nodiscard]] VkDescriptorSetLayout descriptorSetLayout() const {
            return _descriptorSetLayout;
        }

         [[nodiscard]] VkPipeline graphicsPipeline() const {
            return _graphicsPipeline;
        }

         [[nodiscard]] VkPipelineLayout pipelineLayout() const {
            return _pipelineLayout;
        }

    private:
        void createGraphicsPipeline();

        VkShaderModule createShaderModule(const std::vector<char> &code);


        void createDescriptorSetLayout();

    };

} // Enix
