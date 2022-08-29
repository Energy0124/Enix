//
// Created by Energy on 2022/08/10.
//

#pragma once


#include <vulkan/vulkan_core.h>
#include <vector>

#include "Device.h"
#include "WindowSurface.h"
#include "Window.h"
#include "RenderPass.h"

namespace Enix {
    class SwapChain {
        VkSwapchainKHR _swapChain;
        std::vector<VkImage> _swapChainImages;
        std::vector<VkImageView> _swapChainImageViews;
        std::vector<VkFramebuffer> _swapChainFramebuffers;
        VkImage _depthImage;
        VkDeviceMemory _depthImageMemory;
        VkImageView _depthImageView;

        VkFormat _swapChainImageFormat;
        VkExtent2D _swapChainExtent;

        Device &_device;
        WindowSurface &_surface;
        Window &_window;
        RenderPass &_renderPass;

    private:
        void createSwapChain();

        void cleanupSwapChain();

        void createDepthResources();

        void createFramebuffers();

        void createImageViews();

        void createSwapChainResources();

    public:
        SwapChain(Device &device, WindowSurface &surface, Window &window, RenderPass &renderPass);

        ~SwapChain();

        SwapChain(const SwapChain &) = delete;

        SwapChain &operator=(const SwapChain &) = delete;

        void recreateSwapChain();

        [[nodiscard]] VkSwapchainKHR swapChain() const {
            return _swapChain;
        }

        [[nodiscard]] const std::vector<VkImage> &swapChainImages() const {
            return _swapChainImages;
        }

        [[nodiscard]] const std::vector<VkImageView> &swapChainImageViews() const {
            return _swapChainImageViews;
        }

        [[nodiscard]] const std::vector<VkFramebuffer> &swapChainFramebuffers() const {
            return _swapChainFramebuffers;
        }

        [[nodiscard]] VkImage depthImage() const {
            return _depthImage;
        }

        [[nodiscard]] VkDeviceMemory depthImageMemory() const {
            return _depthImageMemory;
        }

        [[nodiscard]] VkImageView depthImageView() const {
            return _depthImageView;
        }

        [[nodiscard]] VkFormat swapChainImageFormat() const {
            return _swapChainImageFormat;
        }

        [[nodiscard]] const VkExtent2D &swapChainExtent() const {
            return _swapChainExtent;
        }
    };

}