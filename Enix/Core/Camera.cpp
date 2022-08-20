//
// Created by Energy on 2022/08/14.
//

#include "Camera.h"

namespace Enix {
    Camera::Camera(const Transform &transform) : Actor(std::string(), transform) {


    }

    glm::mat4 Camera::viewMatrix() {
//        return glm::inverse(transform.modelMatrix());
        front = glm::normalize(front);
        up = glm::normalize(up);
        auto view = glm::lookAt(transform.position, transform.position + front, up);
        // the resulted coordinate system is same as blender, right-handed, z is up, x is right, y is forward

        return view;

    }

    glm::mat4 Camera::projectionMatrix() const {
        auto proj = glm::perspective(fov, aspect, near, far);
        proj[1][1] *= -1;
        return proj;
    }
} // Enix