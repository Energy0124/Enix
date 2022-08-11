//
// Created by Energy on 2022/08/12.
//

#pragma once

#include <string>
#include "stb_image.h"

namespace Enix {

    class TextureAsset {
        int _width{0}, _height{0}, _channels{0};
        std::string _path;
        stbi_uc *_pixels;
    public:
        explicit TextureAsset(std::string path);

        virtual ~TextureAsset();

        [[nodiscard]] const stbi_uc *pixels() const {
            return _pixels;
        }

        [[nodiscard]] int width() const {
            return _width;
        }

        [[nodiscard]] int height() const {
            return _height;
        }

        [[nodiscard]] int channels() const {
            return _channels;
        }

        [[nodiscard]] const std::string &path() const {
            return _path;
        }


    };

} // Enix
