//
// Created by Energy on 2022/08/14.
//

#pragma once

#include "Transform.h"
#include "Asset/MeshAsset.h"

namespace Enix {

    class Actor {
    public:
        explicit Actor(Transform transform);

        Transform transform;


    };

} // Enix
