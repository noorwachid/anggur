#pragma once 

#include "Anggur/Event.h"
#include "Anggur/Math/Vector2.h"
#include "Camera.h"
#include "entt/entt.hpp"

namespace Anggur 
{
    class Entity;
    class Window;
    class MeshRenderer;

    class Scene
    {
    public:
        friend class SceneSystem;
        friend class Entity;
        friend class Application;

        Camera* GetPrimaryCamera()
        {
            auto view = registry.view<Camera>();
            for (auto entity : view)
            {
                return &view.get<Camera>(entity);
            }

            return nullptr;
        };

        Entity Spawn();

        void Attach();

        void Update(float deltaTime);

        void On(Event& event);

        void Detach();

    private:
        entt::registry registry;
        Window* window;
        MeshRenderer* meshRenderer;
        Vector2* viewport;
    };
}
