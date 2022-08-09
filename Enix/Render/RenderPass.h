//
// Created by Energy on 2022/08/10.
//

#pragma once


#include <vulkan/vulkan_core.h>
#include "Device.h"

namespace Enix{
    class RenderPass {
        VkRenderPass _renderPass{VK_NULL_HANDLE};

        VkFormat _swapChainImageFormat = VK_FORMAT_UNDEFINED;

        Device& _device;
    public:
        explicit RenderPass(Device &device);

    private:
        void createRenderPass();

    public:
        virtual ~RenderPass();

        [[nodiscard]] VkRenderPass renderPass() const{
            return _renderPass;
        };
    };

}