#pragma once

#include "SceneManager.h"

namespace Anggur
{
    struct SceneManagerData 
    {
        std::shared_ptr<Scene> activeScene;
    };

    SceneManagerData sceneManagerData;

    void SceneManager::SetActiveScene(const std::shared_ptr<Scene>& scene)
    {
        Terminate();

        sceneManagerData.activeScene = scene;

        Initialize();
    }

    const std::shared_ptr<Scene>& SceneManager::GetActiveScene()
    {
        return sceneManagerData.activeScene;
    }

    void SceneManager::Initialize()
    {
        if (sceneManagerData.activeScene) 
            sceneManagerData.activeScene->Initialize();
    }

    void SceneManager::Update(float deltaTime)
    {
        if (sceneManagerData.activeScene) 
            sceneManagerData.activeScene->Update(deltaTime);
    }

    void SceneManager::Terminate()
    {
        if (sceneManagerData.activeScene) 
            sceneManagerData.activeScene->Terminate();
    }
}