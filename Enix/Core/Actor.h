//
// Created by Energy on 2022/08/14.
//

#pragma once

#include <string>
#include "Transform.h"
#include "Asset/MeshAsset.h"
#include "Input/InputSystem.h"

namespace Enix {

    class Actor {
    public:
        Actor(std::string name, Transform transform);

        std::string name;
        Transform transform;

        virtual void tick(float deltaTime);
        [[nodiscard]] const InputSystem& inputSystem() const;


    };

} // Enix
