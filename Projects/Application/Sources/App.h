#pragma once

#include "Anggur/Graphics/CanvasCamera.h"
#include "Anggur/Graphics/Font.h"
#include "Anggur/Math/Random.h"
#include "BaseApp.h"
#include <iostream>

using namespace Anggur;

struct Mover
{
    Vector2 position;
    Vector2 velocity;
    Vector4 color;
    float desireStrength;

    void Attract(const Vector2& attractor)
    {
        Vector2 localVelocity = Vector2::Normalize(attractor - position);
        velocity += localVelocity;
        velocity.SetLengthLimit(100);
    }

    void Update(float deltaTime)
    {
        position += deltaTime * velocity;
    }

    void Draw(Renderer& renderer)
    {
        Vector2 heading = Vector2::CreatePolar(desireStrength, velocity.GetAngle());
        Vector2 leftWing = Vector2::CreatePolar(desireStrength, velocity.GetAngle() - Math::pi * 0.70);
        Vector2 rightWing = Vector2::CreatePolar(desireStrength, velocity.GetAngle() + Math::pi * 0.70);

        renderer.canvas.DrawQuad(
            Matrix3::CreateTranslation(position), Vector2(0.0f, 0.0f), rightWing, heading, leftWing, color);
    }

    static Mover CreateRandom(const Vector2& spawnPosition = Vector2::zero)
    {
        Random random;

        float maxRadius = 100.0f;
        float maxVelocity = 100.0f;

        Mover mover;
        mover.position = spawnPosition + Vector2::CreatePolar(random.Range(-maxRadius, maxRadius), random.GetAngle());
        mover.velocity = Vector2::CreatePolar(random.Range(Math::epsilon, maxVelocity), random.GetAngle());
        Vector2 bendedVelocity = Vector2::Normalize(mover.velocity.GetPerpendicular());
        bendedVelocity.SetLength(random.Range(Math::epsilon, maxVelocity));
        mover.velocity += bendedVelocity;
        mover.color = Vector4::Lerp(Vector4::createHex(0xD60270), Vector4::createHex(0x0038A8), random.GetNormal());
        mover.desireStrength = random.Range(0.1f, 10.0f);

        return mover;
    }
};

class App : public BaseApp
{
  public:
    std::vector<Mover> movers;

    int moverCount = 10;

    Vector2 mouseDragBeginPoint;
    Vector2 mouseDragEndPoint;
    bool mouseDragging = false;

    Vector2 attractorPosition;

    float attractorAcceleration = 10.0f;
    float attractorMaxVelocity = 1000.0f;
    float attractorVelocity = 0.0f;

    void Initialize() override
    {
        window.SetTitle("TestingCameraApp");

        worldCamera.setScreenSize(window.GetSize());
        worldCamera.setViewSize(0.25 * window.GetSize());

        uiCamera.setScreenSize(window.GetSize());
        uiCamera.setViewSize(window.GetFrameBufferSize());

        font = std::make_shared<Font>();
        font->SetSample("Resources/Fonts/OpenSans-Regular.ttf", 64, 512);
        font->GenerateASCII();

        for (int i = 0; i < moverCount; ++i)
        {
            movers.push_back(std::move(Mover::CreateRandom()));
        }
    }

    void Update(float deltaTime) override
    {
        Keyboard& keyboard = window.GetInputDevice<Keyboard>();
        Mouse& mouse = window.GetInputDevice<Mouse>();

        if (keyboard.IsKeyDown(Key::Space) && mouse.IsButtonPressed(MouseButton::F1))
        {
            mouseDragging = true;
            mouseDragBeginPoint = worldCamera.toWorldPoint(mouse.GetCursorPosition());
        }

        if (mouseDragging && (mouse.IsButtonReleased(MouseButton::F1) || keyboard.IsKeyReleased(Key::Space)))
        {
            mouseDragging = false;
        }

        if (mouseDragging && mouse.IsButtonDown(MouseButton::F1))
        {
            mouseDragEndPoint = worldCamera.toWorldPoint(mouse.GetCursorPosition());
            worldCamera.nudgeTargetPosition(mouseDragEndPoint - mouseDragBeginPoint);
        }

        if (keyboard.IsKeyDown(Key::LeftShift))
        {
            attractorVelocity = Math::Clamp(attractorVelocity + attractorAcceleration, 0.0f, attractorMaxVelocity);
        }

        if (keyboard.IsKeyDown(Key::RightShift))
        {
            attractorVelocity = Math::Clamp(attractorVelocity - attractorAcceleration, 0.0f, attractorMaxVelocity);
        }

        if (keyboard.IsKeyDown(Key::Enter))
        {
            movers.push_back(Mover::CreateRandom(attractorPosition));
        }

        attractorPosition += deltaTime * attractorVelocity * GetInputDeviceAxis();

        worldCamera.nudgeViewScale(deltaTime * mouse.GetWheelDirection().y);
        worldCamera.update();

        renderer.canvas.Clear(Vector4::charcoal);
        renderer.canvas.BeginScene(worldCamera.getViewProjection());

        for (Mover& mover : movers)
        {
            mover.Attract(attractorPosition);
            mover.Update(deltaTime);
            mover.Draw(renderer);
        }

        renderer.canvas.DrawCircle(Matrix3::CreateTranslation(attractorPosition), 10, 6, Vector4::white);

        renderer.canvas.EndScene();
    }

    Vector2 GetInputDeviceAxis()
    {
        Keyboard& keyboard = window.GetInputDevice<Keyboard>();
        Vector2 direction;

        if (keyboard.IsKeyDown(Key::Left))
            direction.x -= 1;

        if (keyboard.IsKeyDown(Key::Right))
            direction.x += 1;

        if (keyboard.IsKeyDown(Key::Up))
            direction.y -= 1;

        if (keyboard.IsKeyDown(Key::Down))
            direction.y += 1;

        return Vector2::Normalize(direction);
    };

  private:
    CanvasCamera worldCamera;
    CanvasCamera uiCamera;
    std::shared_ptr<Font> font;
};