//
// Created by Energy on 2022/08/14.
//

#include "MeshActor.h"

#include <utility>

namespace Enix {
    MeshActor::MeshActor(std::string name, Transform transform, std::shared_ptr<MeshAsset> meshAsset) : Actor(std::move(name),
                                                                                                              transform), _meshAsset(std::move(meshAsset)) {}
}

