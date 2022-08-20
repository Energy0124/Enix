//
// Created by Energy on 2022/08/14.
//

#pragma once

#include "Actor.h"

namespace Enix {

    class Camera : public Actor {

    public:
        explicit Camera(const Transform &transform);

        glm::mat4 viewMatrix();
        glm::mat4 projectionMatrix() const;
        glm::vec3 front{1.0f, 0.0f, 0.0f};
        glm::vec3 right{0.0f, 1.0f, 0.0f};
        glm::vec3 up{0.0f, 0.0f, 1.0f};
        float fov{45.0f};
        float aspect{1.0f};
        float near{0.1f};
        float far{100.0f};
        VkClearColorValue clearColor{0.0f, 0.0f, 0.0f, 1.0f};
    private:



    };

} // Enix
