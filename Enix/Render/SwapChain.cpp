//
// Created by Energy on 2022/08/10.
//
#include <stdexcept>
#include <array>
#include "SwapChain.h"
#include "spdlog/spdlog.h"

namespace Enix {
    void SwapChain::createSwapChain() {

        _window.waitForFocus();

        vkDeviceWaitIdle(_device);

        SwapChainSupportDetails swapChainSupport = _device.querySwapChainSupport(_device.physicalDevice());

        VkSurfaceFormatKHR surfaceFormat = _device.chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = _device.chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = _device.chooseSwapExtent(swapChainSupport.capabilities, &_window.window());

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = _surface.surface();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = _device.findQueueFamilies(_device.physicalDevice());
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }
        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }
        vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, nullptr);
        _swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, _swapChainImages.data());

        _swapChainImageFormat = surfaceFormat.format;
        _swapChainExtent = extent;
    }


    void SwapChain::createImageViews() {
        _swapChainImageViews.resize(_swapChainImages.size());

        for (uint32_t i = 0; i < _swapChainImages.size(); i++) {
            _swapChainImageViews[i] = _device.createImageView(_swapChainImages[i], _swapChainImageFormat,
                                                              VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }

    void SwapChain::createFramebuffers() {
        _swapChainFramebuffers.resize(_swapChainImageViews.size());
        for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
            std::array attachments = {
                    _swapChainImageViews[i],
                    _depthImageView
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = _renderPass.renderPass();
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = _swapChainExtent.width;
            framebufferInfo.height = _swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(_device, &framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    void SwapChain::createDepthResources() {
        VkFormat depthFormat = _device.findDepthFormat();
        _device.createImage(_swapChainExtent.width, _swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL,
                            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                            _depthImage,
                            _depthImageMemory);
        _depthImageView = _device.createImageView(_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
        _device.transitionImageLayout(_depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }

    void SwapChain::cleanupSwapChain() {
        spdlog::debug("cleaning up swap chain");

        vkDestroyImageView(_device, _depthImageView, nullptr);
        vkDestroyImage(_device, _depthImage, nullptr);
        vkFreeMemory(_device, _depthImageMemory, nullptr);

        for (auto &swapChainFramebuffer: _swapChainFramebuffers) {
            vkDestroyFramebuffer(_device, swapChainFramebuffer, nullptr);
        }

        for (auto &swapChainImageView: _swapChainImageViews) {
            vkDestroyImageView(_device, swapChainImageView, nullptr);
        }
        vkDestroySwapchainKHR(_device, _swapChain, nullptr);
    }

    void SwapChain::recreateSwapChain() {
        spdlog::debug("recreate swap chain");
        _window.waitForFocus();
        vkDeviceWaitIdle(_device);
        cleanupSwapChain();

        createSwapChainResources();
    }

    void SwapChain::createSwapChainResources() {
        spdlog::debug("creating swap chain resources");
        createSwapChain();
        createImageViews();
        createDepthResources();
        createFramebuffers();
    }

    SwapChain::SwapChain(Device &device, WindowSurface &surface, Window &window, RenderPass &renderPass)
            : _device(device), _surface(surface), _window(window), _renderPass(renderPass) {

        createSwapChainResources();
    }

    SwapChain::~SwapChain() {
        cleanupSwapChain();
    }




}