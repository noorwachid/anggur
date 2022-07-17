#include "EntityHierarchy.h"

namespace Anggur
{
    struct EntityHierarchyData 
    {
        std::shared_ptr<Entity> rootEntity;
    };

    EntityHierarchyData entityHierarchyData;

    void EntityHierarchy::SetRootEntity(const std::shared_ptr<Entity>& entity)
    {
        Terminate();

        entityHierarchyData.rootEntity = entity;

        Initialize();
    }

    const std::shared_ptr<Entity>& EntityHierarchy::GetRootEntity()
    {
        return entityHierarchyData.rootEntity;
    }

    void EntityHierarchy::Initialize()
    {
        if (entityHierarchyData.rootEntity) 
            entityHierarchyData.rootEntity->Initialize();
    }

    void EntityHierarchy::Update(float deltaTime)
    {
        if (entityHierarchyData.rootEntity) 
        {
            UpdateChildren(entityHierarchyData.rootEntity, deltaTime);
        }
    }

    void EntityHierarchy::UpdateChildren(std::shared_ptr<Entity>& entity, float deltaTime)
    {
        for (auto child: entity->children)
        {
            UpdateChildren(child, deltaTime);
        }
        entity->Update(deltaTime);
    }

    void EntityHierarchy::Terminate()
    {
        if (entityHierarchyData.rootEntity) 
            entityHierarchyData.rootEntity->Terminate();
    }
}