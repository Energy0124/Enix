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
        std::unique_ptr<Model> _model;

        void loadModel(Device &device);

    public:

        MeshAsset(std::string modelPath, std::string texturePath, Device &device);

        ~MeshAsset();

        [[nodiscard]] const Model &model() const {
            return *_model;
        }

        [[nodiscard]] const std::string &texturePath() const {
            return _texturePath;
        }

    };

} // Enix
