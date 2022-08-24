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
        using glm::sin;
        using glm::cos;
        using glm::cross;
        using glm::normalize;
        using glm::dot;
        using glm::radians;



        float yaw = radians(transform.rotation.z);
        float pitch = radians(transform.rotation.x);
        float roll = radians(transform.rotation.y);



        front.x = sin(yaw) * cos(pitch);
        front.y = cos(yaw) * cos(pitch);
        front.z = sin(pitch);


        up.x = -sin(roll) * cos(yaw) -
               cos(roll) * sin(pitch) * sin(yaw);
        up.y = sin(roll) * sin(yaw) -
               cos(roll) * sin(pitch) * cos(yaw);
        up.z = cos(roll) * cos(pitch);

        front = normalize(front);
        up = normalize(up);
        right = normalize(cross(front, up));
        auto view = glm::lookAt(transform.position, transform.position + front, up);
        // the resulted coordinate system is same as blender, right-handed, z is up, x is right, y is forward

        return view;

//        return glm::inverse(transform.translationMatrix() * (transform.rotationMatrix()));

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
        if (inputSystem().isKeyPressed(GLFW_KEY_R)) {
            transform.position += up * deltaTime * speed;
        }
        if (inputSystem().isKeyPressed(GLFW_KEY_F)) {
            transform.position -= up * deltaTime * speed;
        }

    }
} // Enix