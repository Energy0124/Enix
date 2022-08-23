//
// Created by Energy on 2022/08/24.
//

#include "InputSystem.h"

namespace Enix {
    InputSystem::InputSystem(Window &window) : _window(window) {
        _window.registerOnKeyFunc([this](int key, int scanCode, int action, int mods) {
            onKey(key, scanCode, action, mods);
        });
    }

    void InputSystem::onKey(int key, int code, int action, int mods) {
        switch (action) {
            case GLFW_PRESS:
                keyPressed(key);
                break;
            case GLFW_RELEASE:
                keyReleased(key);
                break;
            default:
                break;
        }
    }

    void InputSystem::keyPressed(int key) {
        _pressedKeys[key] = true;
    }

    void InputSystem::keyReleased(int key) {
        _pressedKeys[key] = false;
    }

    bool InputSystem::isKeyPressed(int key) const {
        return _pressedKeys[key];
    }
} // Enix