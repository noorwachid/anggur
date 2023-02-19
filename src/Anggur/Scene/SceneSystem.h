#pragma once 

#include "Scene.h"

namespace Anggur 
{
    class Window;
    class MeshRenderer;

    class SceneSystem
    {
    public:
        void Attach();

        void Attach(Scene* newScene);

        void Update(float deltaTime);

        void On(Event& event);

        void Detach();

    private:
        Scene* scene = nullptr;
        Window* window = nullptr;
        MeshRenderer* meshRenderer = nullptr;
        Vector2* viewport = nullptr;

        friend class Application;
    };
}
