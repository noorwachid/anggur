#pragma once

#include "Anggur/Scene/Node.h"

using namespace Anggur;

class Scene2D: public Node
{
public:
    void Initialize() override
    {
        wallTexture = std::make_shared<Texture2D>("./resources/images/f.png");
    }

    void Update(float deltaTime) override
    {
        position = position + (velocity * deltaTime * GetInputDirection());

        Renderer::Begin(Matrix4::CreateOrthographic(20, 20, 0, 1));

            Renderer::Clear(Vector4(0.1, 0.1, 0.1, 1.0));

            Renderer::RenderPolygon(Matrix4::CreateTranslation(position), 6, Vector4::red);
            
        Renderer::End();
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
};