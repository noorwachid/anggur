#pragma once

#include "Entity.h"
#include <memory>

namespace Anggur
{
    class EntityHierarchy
    {
    public:
        static void SetRootEntity(const std::shared_ptr<Entity>& entity);
        static const std::shared_ptr<Entity>& GetRootEntity();

        static void Initialize();
        static void Update(float deltaTime);
        static void UpdateChildren(std::shared_ptr<Entity>& entity, float deltaTime);

        static void Terminate();
    };    
}