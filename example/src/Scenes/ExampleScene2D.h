#pragma once

#include "Anggur/Scene/Scene.h"

using namespace Anggur;

struct Camera2D
{
    Vector3 position;
    Vector3 target;
};


class ExampleScene2D: public Scene
{
public:
    void Initialize() override
    {
        wallTexture = std::make_shared<Texture2D>("./resources/images/f.png");
    }

    void Update(float deltaTime) override
    {
        Renderer::Begin(Matrix4::CreateOrtho(20, 20, 0, 1));

            Renderer::Clear(Vector4(0.1, 0.1, 0.1, 1.0));

            Renderer::RenderCube(Matrix4::CreateTranslation(Vector3::unitX), wallTexture, Vector4::red);

        Renderer::End();
    }

private:
    std::shared_ptr<Texture2D> wallTexture;

    Camera2D camera;
    Vector3 cameraDirection = Vector3::unitZ;
};