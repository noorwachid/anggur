#pragma once

#include "Anggur/Scene/Entity.h"

using namespace Anggur;

class Scene2D: public Entity
{
public:
    void Initialize() override
    {
        wallTexture = std::make_shared<Texture2D>("./resources/images/f.png");
    }

    void Update(float deltaTime) override
    {
        position = position + (velocity * deltaTime * GetInputDirection());

        renderer.canvas.Begin(Matrix4::CreateOrthographic(20, 20, 0, 1));

            Renderer::GetCanvas().Clear(Vector4(0.1, 0.1, 0.1, 1.0));
            
            Renderer::GetCanvas().RenderRectangle(Matrix4::CreateTranslation(position), Vector3(0, 0, 0), Vector3(3, 5, 0));

            Renderer::GetCanvas().RenderArc(Matrix4::CreateTranslation(Vector3(-5, -5, 0)), 1, Math::pi, Math::pi / 2, 3);
            
        Renderer::GetCanvas().End();

        elapsedTime += deltaTime;
    }

private:
    Vector3 GetInputDirection()
    {
        Vector3 direction;

        if (Input::IsKeyDown(Key::Left))
            direction.x -= 1;
        if (Input::IsKeyDown(Key::Right))
            direction.x += 1;
        if (Input::IsKeyDown(Key::Up))
            direction.y += 1;
        if (Input::IsKeyDown(Key::Down))
            direction.y -= 1;

        return Vector3::Normalize(direction);
    }

private:
    std::shared_ptr<Texture2D> wallTexture;

    Vector3 position;
    float velocity = 3;
    float elapsedTime = 0;
};