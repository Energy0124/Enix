//
// Created by Energy on 2022/08/21.
//

#include "Material.h"
#include "spdlog/spdlog.h"

namespace Enix {
    Material::Material(std::shared_ptr<TextureAsset> textureAsset, Device &device, VkDescriptorPool &descriptorPool,
                       GraphicsPipeline &graphicsPipeline) :
            _textureAsset(std::move(textureAsset)), _device(device), _descriptorPool(descriptorPool),
            _graphicsPipeline(graphicsPipeline), _descriptorSets(1, VK_NULL_HANDLE) {

        spdlog::info("Create Material: {}", _textureAsset->path());
        loadTexture();
        createDescriptorSet();
    }

    void Material::loadTexture() {

        _texture = std::make_unique<Texture>(_device, *_textureAsset);
    }

    void Material::createDescriptorSet() {
        std::vector<VkDescriptorSetLayout> layouts{_graphicsPipeline.descriptorSetLayouts()[1]};

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = _descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = layouts.data();
        if (vkAllocateDescriptorSets(_device, &allocInfo, _descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        // todo: move creation of textureImageView and textureSampler to another class
        imageInfo.imageView = _texture->textureImageView();
        imageInfo.sampler = _texture->textureSampler();

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = _descriptorSets[0];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0,
                               nullptr);
    }

    Material::~Material() {
        spdlog::info("Freeing material: {}", _textureAsset->path());

    }


} // Enix