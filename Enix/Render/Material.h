//
// Created by Energy on 2022/08/21.
//

#pragma once

#include <memory>
#include "Texture.h"
#include "GraphicsPipeline.h"

namespace Enix {

    class Material {

        std::shared_ptr<TextureAsset> _textureAsset;
        std::unique_ptr<Texture> _texture;
        std::vector<VkDescriptorSet> _descriptorSets{};

        Device &_device;
        VkDescriptorPool &_descriptorPool;
        GraphicsPipeline &_graphicsPipeline;

        void loadTexture();

        void createDescriptorSet();

    public:

        Material(std::shared_ptr<TextureAsset> textureAsset, Device &device, VkDescriptorPool &descriptorPool,
                 GraphicsPipeline &graphicsPipeline);

        // virtual destructor as this might be used as a base class later
        virtual ~Material();

        Material(const Material &) = delete;

        Material &operator=(const Material &) = delete;

        [[nodiscard]] const Texture &texture() const {
            return *_texture;
        }

        [[nodiscard]]const std::vector<VkDescriptorSet> &descriptorSets() const {
            return _descriptorSets;
        }

    };

} // Enix
