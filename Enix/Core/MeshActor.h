//
// Created by Energy on 2022/08/14.
//

#pragma once

#include "Actor.h"

namespace Enix {

    class MeshActor : public Actor {
    public:
        MeshActor(std::string name, Transform transform, std::shared_ptr<MeshAsset> meshAsset);

    public:
        [[nodiscard]] std::shared_ptr<MeshAsset> &meshAsset() {
            return _meshAsset;
        }

        void setMeshAsset(std::shared_ptr<MeshAsset> &meshAsset) {
            _meshAsset = meshAsset;
        }

        std::shared_ptr<MeshAsset> _meshAsset;
    };

} // Enix
