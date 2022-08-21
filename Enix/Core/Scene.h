//
// Created by Energy on 2022/08/21.
//

#pragma once


#include <vector>
#include <memory>
#include "Camera.h"
#include "MeshActor.h"

namespace Enix {

    class Scene {

    public:
        std::shared_ptr<Camera> mainCamera = nullptr;
        std::vector<std::shared_ptr<MeshActor>> meshActors{};
        void tick(double deltaTime);
    };

} // Enix
