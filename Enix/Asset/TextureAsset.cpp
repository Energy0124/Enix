//
// Created by Energy on 2022/08/12.
//

#include <stdexcept>
#include <utility>
#include "TextureAsset.h"
#include "spdlog/spdlog.h"

namespace Enix {
    TextureAsset::TextureAsset(std::string  path) :
            _path(std::move(path)) {

        spdlog::info("Loading texture: {}", _path);

        _pixels = stbi_load(_path.data(), &_width, &_height, &_channels, STBI_rgb_alpha);

        if (!_pixels) {
            throw std::runtime_error("failed to load texture image!");
        }

    }

    TextureAsset::~TextureAsset() {
        spdlog::info("Freeing texture: {}", _path);
        stbi_image_free(_pixels);


    }
} // Enix