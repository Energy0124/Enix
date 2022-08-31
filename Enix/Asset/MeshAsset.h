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
        std::unique_ptr<Model> _model;

        void loadModel(Device &device);

    public:

        MeshAsset(std::string modelPath, Device &device);

        [[nodiscard]] const Model &model() const {
            return *_model;
        }

    };

} // Enix
