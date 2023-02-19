#pragma once 

#include <memory>
#include "Anggur/Scene/SceneSystem.h"
#include "Anggur/Scene/Script.h"
#include "Scene/Scene.h"
#include "Scene/Transform.h"
#include "IO/Clock.h"
#include "IO/Window.h"
#include "IO/WindowSystem.h"
#include "Graphics/MeshRenderer.h"

namespace Anggur 
{
    class Application
    {
      public:
        void Initialize()
        {
            window.SetListener([this](Event& event) {
                sceneSystem.On(event);
            });

            meshRenderer.SetViewport(window.GetFrameBufferSize());

            sceneSystem.window = &window;
            sceneSystem.meshRenderer = &meshRenderer;
        }

        void Run(Scene* newScene)
        {
            Initialize();

            sceneSystem.Attach(newScene);

            float previousTime = clock.GetElapsed();

            while (window.IsOpen())
            {
                float currentTime = clock.GetElapsed();
                float deltaTime = currentTime - previousTime;

                windowSystem.PollEvents();

                sceneSystem.Update(deltaTime);

                previousTime = currentTime;

                window.Update();
            }

            sceneSystem.Detach();
        }

      private:
        WindowSystem windowSystem;
        Window window;

        MeshRenderer meshRenderer;
        Clock clock;

        SceneSystem sceneSystem;
    };
}
