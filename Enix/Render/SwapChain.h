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

namespace Enix{
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

        Device& _device;
        WindowSurface& _surface;
        Window& _window;
        RenderPass& _renderPass;
    public:
        SwapChain(Device &device, WindowSurface &surface, Window &window, RenderPass &renderPass);

        virtual ~SwapChain();

        void recreateSwapChain();
    private:
        void createSwapChain();

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);





        void cleanupSwapChain();


        void createDepthResources();

        void createFramebuffers();

        void createImageViews();

    };

}