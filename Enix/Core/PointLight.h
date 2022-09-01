//
// Created by Energy on 2022/09/02.
//

#pragma once

#include "Actor.h"

namespace Enix {

    class PointLight : public Actor {
    public:
        PointLight(const std::string &name, const Transform &transform, glm::vec3 direction, glm::vec3 color)
                : Actor(name, transform), color(color) {}

    public:
        glm::vec3 color{1.0f, 1.0f, 1.0f};
    };

} // Enix
