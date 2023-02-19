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
        virtual void Initialize()
        {
            window.SetListener([this](Event& event) {
                sceneSystem.On(event);
            });

            sceneSystem.window = &window;
            sceneSystem.meshRenderer = &meshRenderer;
            sceneSystem.viewport = &viewport;
        }

        virtual void Run(Scene* newScene)
        {
            Initialize();

            sceneSystem.Attach(newScene);

            clock.Tick();

            while (window.IsOpen())
            {
                windowSystem.PollEvents();

                sceneSystem.Update(clock.Tick());

                window.Update();
            }

            sceneSystem.Detach();
        }

      protected:
        WindowSystem windowSystem;
        Window window;
        Vector2 viewport;

        MeshRenderer meshRenderer;
        Clock clock;

        SceneSystem sceneSystem;
    };
}
