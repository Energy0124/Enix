//
// Created by Energy on 2022/08/14.
//

#include "Camera.h"

namespace Enix {
    Camera::Camera(const Transform &transform) : Actor(std::string(), transform) {


    }

    glm::mat4 Camera::viewMatrix() {
//        return glm::inverse(transform.modelMatrix());
        // todo: update front, right, up according to transform.rotation

        front.y = glm::cos(glm::radians(transform.rotation.x)) * glm::cos(glm::radians(transform.rotation.y));
        front.z = glm::sin(glm::radians(transform.rotation.y));
        front.x = glm::sin(glm::radians(transform.rotation.x)) * glm::cos(glm::radians(transform.rotation.y));
        front = glm::normalize(front);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
        auto view = glm::lookAt(transform.position, transform.position + front, up);
        // the resulted coordinate system is same as blender, right-handed, z is up, x is right, y is forward

        return view;

    }

    glm::mat4 Camera::projectionMatrix() const {
        auto proj = glm::perspective(fov, aspect, near, far);
        proj[1][1] *= -1;
        return proj;
    }

    void Camera::tick(float deltaTime) {
        Actor::tick(deltaTime);
        float speed = 1.f;
        if (inputSystem().isKeyPressed(GLFW_KEY_W)) {
            transform.position += front * deltaTime * speed;
        }
        if (inputSystem().isKeyPressed(GLFW_KEY_S)) {
            transform.position -= front * deltaTime * speed;
        }
        if (inputSystem().isKeyPressed(GLFW_KEY_A)) {
            transform.position -= right * deltaTime * speed;
        }
        if (inputSystem().isKeyPressed(GLFW_KEY_D)) {
            transform.position += right * deltaTime * speed;
        }

    }
} // Enix