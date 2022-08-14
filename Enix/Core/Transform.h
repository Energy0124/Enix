//
// Created by Energy on 2022/08/14.
//

#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Enix {

    struct Transform {
        glm::vec3 position{};
        glm::vec3 rotation{};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};

        glm::mat4 modelMatrix();
    };

} // Enix
