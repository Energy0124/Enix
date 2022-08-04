//
// Created by Energy on 2022/08/05.
//

#pragma once

#include <string>
#include <memory>
#include "Render/Model.h"

namespace Enix {

    class MeshAsset {
        std::string _modelPath;
        std::string _texturePath;
        std::unique_ptr<Model> _model;

        void loadModel(const Device &device);

        void loadTexture();

    public:

        MeshAsset(std::string modelPath, std::string texturePath, const Device &device);

        ~MeshAsset();

        [[nodiscard]] const Model &model() const {
            return *_model;
        }
    };

} // Enix
