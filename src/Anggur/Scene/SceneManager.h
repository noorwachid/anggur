#pragma once

#include "Scene.h"
#include <memory>

namespace Anggur
{
    class SceneManager
    {
    public:
        static void SetActiveScene(const std::shared_ptr<Scene>& scene);
        static const std::shared_ptr<Scene>& GetActiveScene();

        static void Initialize();
        static void Update(float deltaTime);
        static void Terminate();
    };    
}