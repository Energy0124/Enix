//
// Created by Energy on 2022/08/18.
//

#pragma once

// forward declarations
namespace Enix{
    class VulkanEngine;
}

namespace Enix {

    class AppUI {

    public:

        explicit AppUI(VulkanEngine &vulkanEngine);

        void onDraw();

    private:
        bool _showDemo;

        VulkanEngine& _engine;

        void CameraControl();

        void ActorControls();
    };

} // Enix
