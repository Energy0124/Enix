//
// Created by Energy on 2022/08/14.
//

#pragma once

#include "Actor.h"
#include "Render/Material.h"

namespace Enix {

    class MeshActor : public Actor {
        std::shared_ptr<MeshAsset> _meshAsset;
        std::shared_ptr<Material> _material;
    public:
        MeshActor(std::string name, Transform transform, std::shared_ptr<MeshAsset> meshAsset,
                  std::shared_ptr<Material> material);

    public:
        [[nodiscard]] std::shared_ptr<MeshAsset> &meshAsset() {
            return _meshAsset;
        }

        void setMeshAsset(std::shared_ptr<MeshAsset> &meshAsset) {
            _meshAsset = meshAsset;
        }

        [[nodiscard]] const std::shared_ptr<Material> &material() const{
            return _material;
        }

    };

} // Enix
