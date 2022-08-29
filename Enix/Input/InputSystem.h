//
// Created by Energy on 2022/08/24.
//

#pragma once

#include "Render/Window.h"

namespace Enix {

    class InputSystem {


    private:
        std::array<bool, GLFW_KEY_LAST> _pressedKeys{false};
        Window &_window;

        void onKey(int key, int code, int action, int mods);

        void keyPressed(int key);

        void keyReleased(int key);

    public:
        explicit InputSystem(Window &window);

        ~InputSystem();

        [[nodiscard]] bool isKeyPressed(int key) const;

    };

} // Enix
