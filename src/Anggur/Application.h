#pragma once 

#include <memory>
#include "Scene/Entity.h"
#include "Scene/Registry.h"
#include "IO/Clock.h"
#include "IO/Window.h"
#include "IO/WindowSystem.h"
#include "Graphics/MeshRenderer.h"

namespace Anggur 
{
    class Application
    {
      public:
        void Initialize(Entity* newEntity = nullptr)
        {
            entity = newEntity;
            entity->registry = &registry;
            entity->window = &window;

            window.SetListener([this](Event& event) {
                if (entity)
                    entity->On(event);
            });

            renderer.SetViewport(window.GetFrameBufferSize());
        }

        void Run(Entity* newEntity)
        {
            Initialize(newEntity);

            if (entity)
                entity->Initialize();

            float previousTime = clock.GetElapsed();

            while (window.IsOpen())
            {
                float currentTime = clock.GetElapsed();

                windowSystem.PollEvents();

                if (entity)
                {
                    Camera* camera = registry.GetPrimaryCamera();

                    float deltaTime = currentTime - previousTime;
                    float cameraSpeed = 5.f * deltaTime;

                    auto windowSize = window.GetSize();
                    auto projection = Matrix4::CreatePerspective(Math::DegreeToRadian(75.0f), windowSize.x, windowSize.y, 0.1f, 100.f);
                    auto vp = projection;

                    if (camera) 
                    {
                        if (window.keyboard.IsKeyDown(Key::Up))
                            camera->position += cameraSpeed * camera->front;

                        if (window.keyboard.IsKeyDown(Key::Down))
                            camera->position -= cameraSpeed * camera->front;

                        if (window.keyboard.IsKeyDown(Key::Left))
                            camera->position += Vector3::Normalize(Vector3::Cross(camera->front, camera->up)) * cameraSpeed;

                        if (window.keyboard.IsKeyDown(Key::Right))
                            camera->position -= Vector3::Normalize(Vector3::Cross(camera->front, camera->up)) * cameraSpeed;

                        auto view = Matrix4::CreateLookAt(camera->position, camera->position + camera->front, camera->up);
                        vp = view * projection;
                    }


                    renderer.BeginScene(vp);

                    renderer.Clear(Vector4::charcoal);

                    entity->Update(deltaTime);

                    std::vector<Vector3> cubePositions = {
                        Vector3( 0.0f,  0.0f,  0.0f), 
                        Vector3( 2.0f,  5.0f, -15.0f), 
                        Vector3(-1.5f, -2.2f, -2.5f),  
                        Vector3(-3.8f, -2.0f, -12.3f),  
                        Vector3( 2.4f, -0.4f, -3.5f),  
                        Vector3(-1.7f,  3.0f, -7.5f),  
                        Vector3( 1.3f, -2.0f, -2.5f),  
                        Vector3( 1.5f,  2.0f, -2.5f), 
                        Vector3( 1.5f,  0.2f, -1.5f), 
                        Vector3(-1.3f,  1.0f, -1.5f) 
                    };

                    for (size_t i = 0; i < cubePositions.size(); ++i) 
                    {
                        float angle = 20.f * i; 
                        Vector3 translation = cubePositions[i];
                        Quaternion quaternion(Vector3(1.f, 0.3f, 0.5f), Math::DegreeToRadian(angle));

                        auto model = Matrix4::CreateRotation(quaternion) * Matrix4::CreateTranslation(translation);
                        auto color = Vector4::burgundy;

                        renderer.DrawCube(model, color);
                    }

                    renderer.EndScene();
                }

                previousTime = currentTime;

                window.Update();
            }

            if (entity)
                entity->Terminate();
        }

      private:
        WindowSystem windowSystem;
        Window window;

        MeshRenderer renderer;
        Clock clock;

        Entity* entity;
        Registry registry;
    };
}
