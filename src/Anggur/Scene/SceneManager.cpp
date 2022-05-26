#pragma once

#include <Anggur/Scene/SceneManager.h>

namespace Anggur {
    static std::shared_ptr<Scene> scene;

    void SceneManager:: set(const std::shared_ptr<Scene>& newScene) {
        if (scene)
            scene->terminate();

        scene = newScene;
        scene->initialize();
    }

    const std::shared_ptr<Scene>& SceneManager::get() {
        return scene;
    }
}