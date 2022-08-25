//
// Created by Energy on 2022/08/13.
//
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <set>
#include <algorithm>
#include <chrono>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>


#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <spdlog/spdlog.h>

#include "Asset/MeshAsset.h"
#include "Render/VulkanUtility.h"
#include "Render/GraphicsPipeline.h"
#include "VulkanEngine.h"
#include "Renderer.h"
#include "Core/MeshActor.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>


namespace Enix {
    Renderer::Renderer(VulkanEngine &engine) :
            _engine(engine),
            _window(engine.window()),
            _instance(_enableValidationLayers),
            _surface(_instance.instance(), _window.window()),
            _device(_enableValidationLayers, _instance.instance(),
                    _surface.surface()),
            _swapChain(_device, _surface, _window, _renderPass),
            _renderPass(_device),
            _graphicsPipeline(_workspaceRoot, _device, _swapChain, _renderPass) {
        spdlog::debug("init engine");

        init();
    }

    Renderer::~Renderer() {
        // try catch to avoid leaking exceptions in destructor
        try {
            cleanUp();
        }
        catch (const std::exception &e) {
            spdlog::error("Exception in VulkanEngine destructor: {}", e.what());
        }
    }

    void Renderer::draw() {
        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        drawUI();
        ImGui::Render();

        // Update and Render additional Platform Windows
        if (_imguiIo->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        updateCamera();
        drawFrame();
    }

    void Renderer::drawUI() {

        _engine.drawUI();
    }


    void Renderer::updateUniformBuffer(uint32_t currentImage) {
        UniformBufferObject ubo{};
        ubo.view = _camera->viewMatrix();
        ubo.proj = _camera->projectionMatrix();
        ubo.cameraPosition = _camera->transform.position;

        void *data;
        vkMapMemory(_device, _uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(_device, _uniformBuffersMemory[currentImage]);
    }


    void Renderer::drawFrame() {
        vkWaitForFences(_device, 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);
        vkResetFences(_device, 1, &_inFlightFences[_currentFrame]);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(_device, _swapChain.swapChain(), UINT64_MAX,
                                                _imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE,
                                                &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            _swapChain.recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        updateUniformBuffer(_currentFrame);

        vkResetCommandBuffer(_commandBuffers[_currentFrame], 0);
        recordCommandBuffer(_commandBuffers[_currentFrame], imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {_imageAvailableSemaphores[_currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &_commandBuffers[_currentFrame];

        VkSemaphore signalSemaphores[] = {_renderFinishedSemaphores[_currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(_device.graphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {_swapChain.swapChain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        result = vkQueuePresentKHR(_device.presentQueue(), &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window.framebufferResized()) {
            _window.setFramebufferResized(false);
            _swapChain.recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        _currentFrame = (_currentFrame + 1) % _maxFramesInFlight;
    }

    void Renderer::createCommandBuffers() {
        _commandBuffers.resize(_maxFramesInFlight);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = _device.commandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

        if (vkAllocateCommandBuffers(_device, &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void Renderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = _renderPass.renderPass();
        renderPassInfo.framebuffer = _swapChain.swapChainFramebuffers()[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = _swapChain.swapChainExtent();

        std::array<VkClearValue, 2> clearValues = {};
        clearValues[0].color = _camera->clearColor;
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline.graphicsPipeline());


        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(_swapChain.swapChainExtent().width);
        viewport.height = static_cast<float>(_swapChain.swapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = _swapChain.swapChainExtent();
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline.pipelineLayout(), 0,
                                1,
                                &_descriptorSets[_currentFrame], 0, nullptr);

        for (auto &actor: _meshActors) {
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline.pipelineLayout(),
                                    1,
                                    1,
                                    &actor->material()->descriptorSets()[0], 0, nullptr);
            actor->meshAsset()->model().bind(commandBuffer);
            MeshPushConstant constant{actor->transform.modelMatrix(), actor->transform.normalMatrix()};
            //upload the matrix to the GPU via push constant
            vkCmdPushConstants(_commandBuffers[_currentFrame], _graphicsPipeline.pipelineLayout(),
                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                               sizeof(MeshPushConstant), &constant);
            actor->meshAsset()->model().draw(commandBuffer);
        }


        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), _commandBuffers[_currentFrame]);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void Renderer::createSyncObjects() {
        _imageAvailableSemaphores.resize(_maxFramesInFlight);
        _renderFinishedSemaphores.resize(_maxFramesInFlight);
        _inFlightFences.resize(_maxFramesInFlight);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < _maxFramesInFlight; i++) {
            if (vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(_device, &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create semaphores!");
            }
        }
    }


    void Renderer::createUniformBuffers() {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        _uniformBuffers.resize(_maxFramesInFlight);
        _uniformBuffersMemory.resize(_maxFramesInFlight);
        for (size_t i = 0; i < _maxFramesInFlight; i++) {
            _device.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                 _uniformBuffers[i], _uniformBuffersMemory[i]);
        }
    }

    void Renderer::createDescriptorPool() {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(_maxFramesInFlight) * 256;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(_maxFramesInFlight) * 256;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(_maxFramesInFlight) * 256;
        if (vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    void Renderer::createDescriptorSets() {
        std::vector<VkDescriptorSetLayout> layouts(_maxFramesInFlight, _graphicsPipeline.descriptorSetLayouts()[0]);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = _descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(_maxFramesInFlight);
        allocInfo.pSetLayouts = layouts.data();
        _descriptorSets.resize(_maxFramesInFlight);
        VkResult result = vkAllocateDescriptorSets(_device, &allocInfo, _descriptorSets.data());
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < _maxFramesInFlight; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = _uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);


            std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = _descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;
            descriptorWrites[0].pImageInfo = nullptr; // Optional
            descriptorWrites[0].pTexelBufferView = nullptr; // Optional

            vkUpdateDescriptorSets(_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0,
                                   nullptr);
        }
    }

    void Renderer::initImgui() {
        //1: create descriptor pool for IMGUI
        // the size of the pool is very oversize, but it's copied from imgui demo itself.
        VkDescriptorPoolSize poolSizes[] =
                {
                        {VK_DESCRIPTOR_TYPE_SAMPLER,                1000},
                        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000},
                        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000},
                        {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000},
                        {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000},
                        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000},
                        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000},
                        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                        {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000}
                };
        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000;
        poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
        poolInfo.pPoolSizes = poolSizes;


        if (vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_imguiDescriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create imgui descriptor pool!");
        }

        //2 init imgui
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        _imguiIo = &ImGui::GetIO();
        (void) _imguiIo;
        _imguiIo->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        _imguiIo->ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        _imguiIo->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle &style = ImGui::GetStyle();
        if (_imguiIo->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }


        QueueFamilyIndices indices = _device.findQueueFamilies(_device.physicalDevice());

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(&_window.window(), true);
        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = _instance.instance();
        initInfo.PhysicalDevice = _device.physicalDevice();
        initInfo.Device = _device;
        initInfo.QueueFamily = indices.graphicsFamily.value();
        initInfo.Queue = _device.presentQueue();
        initInfo.PipelineCache = VK_NULL_HANDLE;
        initInfo.DescriptorPool = _imguiDescriptorPool;
        initInfo.Subpass = 0;
        initInfo.MinImageCount = _maxFramesInFlight;
        initInfo.ImageCount = static_cast<uint32_t>(_swapChain.swapChainImages().size());
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        initInfo.Allocator = VK_NULL_HANDLE;
        initInfo.CheckVkResultFn = VK_NULL_HANDLE;
        ImGui_ImplVulkan_Init(&initInfo, _renderPass.renderPass());

        VkCommandBuffer commandBuffer = _device.beginSingleTimeCommands();
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        _device.endSingleTimeCommands(commandBuffer);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    void Renderer::initVulkan() {

        // Setup Vulkan
        createUniformBuffers();
        createDescriptorPool();
        createCommandBuffers();
        createSyncObjects();

        createDescriptorSets();

    }

    void Renderer::createRenderObjects(std::shared_ptr<Scene> scene) {


        auto meshAsset = std::make_shared<MeshAsset>(_workspaceRoot + _modelPath,
                                                 _workspaceRoot + _texturePath, _device);
        auto meshAsset2 = std::make_shared<MeshAsset>(_workspaceRoot + _model2Path,
                                                  _workspaceRoot + _texture2Path, _device);

        auto textureAsset = std::make_shared<TextureAsset>(meshAsset->texturePath());
        auto textureAsset2 = std::make_shared<TextureAsset>(meshAsset2->texturePath());

        auto material = std::make_shared<Material>(textureAsset, _device, _descriptorPool, _graphicsPipeline);
        auto material2 = std::make_shared<Material>(textureAsset2, _device, _descriptorPool, _graphicsPipeline);

        Transform t = {{0, 0, 0},
                       {0, 0, 0},
                       {1, 1, 1}};
        auto actor = std::make_shared<MeshActor>("actor 1", t, meshAsset, material);
        _meshActors.push_back(actor);
        _meshActors.push_back(std::make_unique<MeshActor>("actor 2", Transform{{5, 0, 0},
                                                                               {0, 0, 0},
                                                                               {1, 1, 1}}, meshAsset, material));
        _meshActors.push_back(std::make_unique<MeshActor>("actor 3", Transform{{0, 7, 0},
                                                                               {0, 0, 0},
                                                                               {1, 1, 1}}, meshAsset2, material2));
        _camera = std::make_unique<Camera>(
                Transform({{5.0f, 10.0f, 2.0f},
                           {0.f, 0.f, -180},
                           {1,    1,     1}}));
        _camera->front = {-2.0f, -2.0f, -2.0f};

        scene->mainCamera = _camera;
        scene->meshActors = _meshActors;
    }

    void Renderer::init() {
        initVulkan();

        initImgui();
    }

    void Renderer::cleanUp() {

        spdlog::debug("Cleaning up renderer");

        vkDeviceWaitIdle(_device);

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        for (size_t i = 0; i < _maxFramesInFlight; i++) {
            vkDestroyBuffer(_device, _uniformBuffers[i], nullptr);
            vkFreeMemory(_device, _uniformBuffersMemory[i], nullptr);
        }

        vkDestroyDescriptorPool(_device, _imguiDescriptorPool, nullptr);
        vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);

        for (size_t i = 0; i < _maxFramesInFlight; ++i) {
            vkDestroySemaphore(_device, _imageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(_device, _renderFinishedSemaphores[i], nullptr);
            vkDestroyFence(_device, _inFlightFences[i], nullptr);
        }
    }

    void Renderer::updateCamera() {
        _camera->aspect = _swapChain.swapChainExtent().width / static_cast<float>(_swapChain.swapChainExtent().height);
    }

} // Enix