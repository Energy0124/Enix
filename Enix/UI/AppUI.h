//
// Created by Energy on 2022/08/18.
//

#pragma once

// forward declarations
namespace Enix {
    class VulkanEngine;
}

namespace Enix {

    class AppUI {

        bool _showDemo = false;

        VulkanEngine &_engine;

        void CameraControl();

        void ActorControls();

        void LightControl();
    public:

        explicit AppUI(VulkanEngine &vulkanEngine);

        virtual ~AppUI() = default;

        void onDraw();

    };

} // Enix
