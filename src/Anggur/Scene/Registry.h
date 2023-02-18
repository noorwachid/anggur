#pragma once 

#include "Anggur/Scene/Camera.h"
#include "entt/entt.hpp"

namespace Anggur 
{
    class Registry
    {
    public:
        friend class Entity;

        Camera* GetPrimaryCamera()
        {
            auto view = database.view<Camera>();
            for (auto entity : view)
            {
                return &view.get<Camera>(entity);
            }

            return nullptr;
        };

    private:
        entt::registry database;
    };
}
