//
// Created by Energy on 2022/08/14.
//

#include "Actor.h"
#include "MeshActor.h"
#include "VulkanEngine.h"

#include <utility>

namespace Enix {
    Actor::Actor(std::string name, Transform transform)
            : transform(transform), name(std::move(name)) {}

    void Actor::tick(float deltaTime) {

    }

    const InputSystem &Actor::inputSystem() const {
       return VulkanEngine::getInstance().inputSystem();
    }
} // Enix