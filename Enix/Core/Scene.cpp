//
// Created by Energy on 2022/08/21.
//

#include "Scene.h"

namespace Enix {
    void Scene::tick(double deltaTime) {
        for (auto &actor: meshActors) {
            actor->tick(deltaTime);
        }
        mainCamera->tick(deltaTime);
    }
} // Enix