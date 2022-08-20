//
// Created by Energy on 2022/08/14.
//

#include "Transform.h"

namespace Enix {
    glm::mat4 Transform::modelMatrix() {

        return scaleMatrix() * rotationMatrix() * translationMatrix();
    }

    glm::mat4 Transform::rotationMatrix() {
        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        return rotationMatrix;
    }

    glm::mat4 Transform::translationMatrix() {
        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, position);
        return m;
    }

    glm::mat4 Transform::scaleMatrix() {
        glm::mat4 m = glm::mat4(1.0f);
        m = glm::scale(m, scale);
        return m;
    }
} // Enix