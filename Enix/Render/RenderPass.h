//
// Created by Energy on 2022/08/10.
//

#pragma once


#include <vulkan/vulkan_core.h>
#include "Device.h"

namespace Enix {
    class RenderPass {
        VkRenderPass _renderPass{VK_NULL_HANDLE};

        VkFormat _swapChainImageFormat = VK_FORMAT_UNDEFINED;

        Device &_device;

    private:
        void createRenderPass();

        void createImGuiRenderPass();

    public:
        explicit RenderPass(Device &device);

        explicit RenderPass(Device &device, bool isImgui);

        virtual ~RenderPass();

        RenderPass(const RenderPass &) = delete;

        RenderPass &operator=(const RenderPass &) = delete;

        [[nodiscard]] VkRenderPass renderPass() const {
            return _renderPass;
        }

        [[nodiscard]] VkFormat swapChainImageFormat() const {
            return _swapChainImageFormat;
        }

    };

}