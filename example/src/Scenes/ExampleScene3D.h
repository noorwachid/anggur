#pragma once

#if 0
#include "Anggur/Scene/Scene.h"

using namespace Anggur;

struct Camera
{
    Vector3 position;
    Vector3 target;
    Vector3 down;

    float fov = Math::DegreeToRadian(70.f);

    Matrix4 ToMatrix4()
    {
        return 
            Matrix4::CreateLookAt(position, target, down) *
            Matrix4::CreatePerspective(fov, 10, 10, 0.1, 100);
    }
};


class ExampleScene3D: public Scene
{
public:
    void Initialize() override
    {
        wallTexture = std::make_shared<Texture2D>("./resources/images/f.png");

        camera.position.Set(0, 0, -10);
    }

    void Update(float deltaTime) override
    {
        float cameraMovementVelocity = 12.5;
        float cameraFovVelocity = 1.5;

        if (Input::IsKeyDown(Key::Left))
            camera.position.x -= cameraMovementVelocity * deltaTime;
        else if (Input::IsKeyDown(Key::Right))
            camera.position.x += cameraMovementVelocity * deltaTime;
        else if (Input::IsKeyDown(Key::Up))
            camera.position.z += cameraMovementVelocity * deltaTime;
        else if (Input::IsKeyDown(Key::Down))
            camera.position.z -= cameraMovementVelocity * deltaTime;

        float scrollDirectionY = Input::GetScrollDirection().y;
        camera.fov += scrollDirectionY * cameraFovVelocity * deltaTime;
        camera.fov = Math::Clamp(camera.fov, Math::DegreeToRadian(1), Math::DegreeToRadian(70));

        camera.target = camera.position + cameraDirection;
        camera.down.Set(0, 1, 0);

        Renderer::Begin(camera.ToMatrix4());

            Renderer::Clear(Vector4(0.1, 0.1, 0.1, 1.0));

            Renderer::RenderCube(Matrix4::CreateTranslation(Vector3::unitX + Vector3(0, 0, 10)), wallTexture, Vector4::red);
            Renderer::RenderCube(Matrix4::CreateTranslation(Vector3::unitY + Vector3(0, 0, 10)), wallTexture, Vector4::blue);
            Renderer::RenderCube(Matrix4::CreateTranslation(Vector3::unitZ + Vector3(0, 0, 10)), wallTexture, Vector4::green);

        Renderer::End();
    }

private:
    std::shared_ptr<Texture2D> wallTexture;

    Camera camera;
    Vector3 cameraDirection = Vector3::unitZ;
};
#endif