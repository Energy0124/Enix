//
// Created by Energy on 2022/08/14.
//

#include "Camera.h"

namespace Enix {
    Camera::Camera(const Transform &transform) : Actor(transform) {


    }

    glm::mat4 Camera::viewMatrix() const {
        return glm::lookAt(transform.position, transform.position + front, up);

    }

    glm::mat4 Camera::projectionMatrix() const {
        auto proj = glm::perspective(fov, aspect, near, far);
        proj[1][1] *= -1;
        return proj;
    }
} // Enix