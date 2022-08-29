//
// Created by Energy on 2022/08/12.
//

#pragma once

#include "Device.h"
#include "Asset/TextureAsset.h"

namespace Enix {

    class Texture {
        VkImage _textureImage;
        VkDeviceMemory _textureImageMemory;
        VkSampler _textureSampler;
        VkImageView _textureImageView;

        Device &_device;
        TextureAsset &_textureAsset;

    private:
        void createTexture();

        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        void createTextureSampler();

        void createTextureImageView();

    public:
        Texture(Device &device, TextureAsset &pixels);

        virtual ~Texture();

        Texture(const Texture &) = delete;

        Texture &operator=(const Texture &) = delete;

        [[nodiscard]] VkImage textureImage() const {
            return _textureImage;
        }

        [[nodiscard]] VkDeviceMemory textureImageMemory() const {
            return _textureImageMemory;
        }

        [[nodiscard]] VkSampler textureSampler() const {
            return _textureSampler;
        }

        [[nodiscard]] VkImageView textureImageView() const {
            return _textureImageView;
        }

    };

} // Enix
