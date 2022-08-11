//
// Created by Energy on 2022/08/05.
//

#pragma once

#include <string>
#include <memory>
#include "Render/Model.h"
#include "Render/Texture.h"
#include "TextureAsset.h"

namespace Enix {

    class MeshAsset {
        std::string _modelPath;
        std::string _texturePath;

        TextureAsset _textureAsset;

        std::unique_ptr<Model> _model;
        std::unique_ptr<Texture> _texture;

        void loadModel(Device &device);
        void loadTexture(Device &device);


    public:

        MeshAsset(std::string modelPath, std::string texturePath, Device &device);

        ~MeshAsset();

        [[nodiscard]] const Model &model() const {
            return *_model;
        }

        [[nodiscard]] const Texture &texture() const {
            return *_texture;
        }

    };

} // Enix
