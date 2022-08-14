//
// Created by Energy on 2022/08/14.
//

#include "MeshActor.h"

#include <utility>

namespace Enix {
    MeshActor::MeshActor(Enix::Transform transform, std::shared_ptr<MeshAsset> meshAsset) : Actor(
            transform), _meshAsset(std::move(meshAsset)) {}
}

