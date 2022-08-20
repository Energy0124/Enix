//
// Created by Energy on 2022/08/14.
//

#include "Actor.h"
#include "MeshActor.h"

#include <utility>

namespace Enix {
    Actor::Actor(std::string name, Transform transform)
            : transform(transform), name(std::move(name)) {}
} // Enix