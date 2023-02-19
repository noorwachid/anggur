#include "Scene.h"
#include "Anggur/Graphics/MeshRenderer.h"
#include "Anggur/IO/Window.h"
#include "Anggur/Scene/Script.h"
#include "Anggur/Scene/Transform.h"
#include "Entity.h"

namespace Anggur 
{
    Entity Scene::Spawn()
    {
        Entity entity(registry.create());
        entity.scene = this;

        return entity;
    }

    void Scene::Attach()
    {
        auto scriptView = registry.view<Script>();

        for (auto [id, script] : scriptView.each())
        {
            // TODO: Move this to on attach script event
            if (script.instance == nullptr) 
            {
                script.instance = script.spawn();
                script.instance->id = id;
                script.instance->scene = this;
                script.instance->window = window;
                script.instance->Create();
            }
        }
    }

    void Scene::Update(float deltaTime)
    {
        auto scriptView = registry.view<Script>();

        for (auto [id, script] : scriptView.each())
        {
            script.instance->Update(deltaTime);
        }

        auto windowSize = window->GetSize();
        auto projection = Matrix4::CreatePerspective(Math::DegreeToRadian(75.0f), windowSize.x, windowSize.y, 0.1f, 100.f);

        Camera* camera = GetPrimaryCamera();

        auto viewProjection = camera 
            ? Matrix4::CreateLookAt(camera->position, camera->position + camera->front, camera->up) * projection
            : projection;

        meshRenderer->BeginScene(viewProjection);

        meshRenderer->Clear(Vector4::charcoal);

        auto transformMeshView = registry.view<Transform, Mesh>();

        for (auto [id, transform, mesh] : transformMeshView.each())
        {
            meshRenderer->Draw(transform.ToMatrix(), mesh);
        }

        meshRenderer->EndScene();
    }

    void Scene::On(Event& event)
    {
    }

    void Scene::Detach()
    {

        auto scriptView = registry.view<Script>();

        for (auto [id, script] : scriptView.each())
        {
            script.instance->Destroy();
            script.kill(script.instance);
        }
    }
}
