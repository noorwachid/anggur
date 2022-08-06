#pragma once

#include "BaseApp.h"
#include "Anggur/Graphics/CanvasCamera.h"
#include "Anggur/Graphics/Font.h"
#include "Anggur/Math/Random.h"
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
		Vector2 localVelocity =  Vector2::Normalize(attractor - position);
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

		renderer.canvas.DrawQuad(Matrix3::CreateTranslation(position), Vector2(0.0f, 0.0f), rightWing, heading, leftWing, color);
	}
};

class App: public BaseApp 
{
public:
	Random random;
	std::vector<Mover> movers;

	int moverCount = 1000;

	Vector2 mouseDragBeginPoint;
	Vector2 mouseDragEndPoint;
	bool mouseDragging = false;


	Vector2 attractorPosition;
	Vector2 attractorVelocity;
	float attractorAcceleration = 0.0f;
	float attractorMaxVelocity = 1000.0f;

	void Initialize() 
	{
		worldCamera.SetScreenSize(window.GetSize());
		worldCamera.SetViewSize(window.GetFrameBufferSize());

		uiCamera.SetScreenSize(window.GetSize());
		uiCamera.SetViewSize(window.GetFrameBufferSize());
	
		font = std::make_shared<Font>();
        font->SetSample("resources/fonts/OpenSans-Regular.ttf", 64, 512);
        font->GenerateASCII();

		float maxRadius = Math::Max(window.GetFrameBufferSize().x, window.GetFrameBufferSize().y) / 2;
		float maxVelocity = 50.0f;

		for (int i = 0; i < moverCount; ++i) 
		{
			Mover mover;
			mover.position = Vector2::CreatePolar(random.Range(-maxRadius, maxRadius), random.GetAngle());
			mover.velocity = Vector2::CreatePolar(random.Range(Math::epsilon, maxVelocity), random.GetAngle() * random.Range(0.01f, 10.0f));
			Vector2 bendedVelocity = Vector2::Normalize(mover.velocity.GetPerpendicular());
			bendedVelocity.SetLength(random.Range(Math::epsilon, maxVelocity));
			mover.velocity += bendedVelocity;
			mover.color = Vector4::Lerp(Vector4::CreateHex(0xD60270), Vector4::CreateHex(0x0038A8), random.GetNormal());
			mover.desireStrength = random.Range(0.1f, 10.0f);
			movers.push_back(std::move(mover));
		}
	}

	void Update(float deltaTime) 
	{
		if (window.GetInput<Keyboard>().IsKeyDown(Key::Space) && window.GetInput<Mouse>().IsButtonPressed(MouseButton::F1))
		{
			mouseDragging = true;
			mouseDragBeginPoint = worldCamera.ToWorldPoint(window.GetInput<Mouse>().GetPosition());
		}

		if (mouseDragging && (window.GetInput<Mouse>().IsButtonReleased(MouseButton::F1) || window.GetInput<Keyboard>().IsKeyReleased(Key::Space)))
		{
			mouseDragging = false;
		}

		if (mouseDragging && window.GetInput<Mouse>().IsButtonDown(MouseButton::F1))
		{
			mouseDragEndPoint = worldCamera.ToWorldPoint(window.GetInput<Mouse>().GetPosition());
			worldCamera.NudgeTargetPosition(mouseDragEndPoint - mouseDragBeginPoint);
		}

		if (window.GetInput<Keyboard>().IsKeyDown(Key::LeftShift))
		{
			attractorAcceleration += Math::Clamp(attractorAcceleration + 1.0f, 0.0f, attractorMaxVelocity);
			attractorVelocity += deltaTime * attractorAcceleration;
		}
		if (window.GetInput<Keyboard>().IsKeyDown(Key::RightShift))
		{
			attractorAcceleration += Math::Clamp(attractorAcceleration - 1.0f, 0.0f, attractorMaxVelocity);
			attractorVelocity += deltaTime * attractorAcceleration;
		}

		attractorPosition += deltaTime * attractorVelocity * GetInputAxis();

		worldCamera.NudgeViewScale(deltaTime * window.GetInput<Scroller>().GetDirection().y);
		worldCamera.Update();

		renderer.canvas.Clear(Vector4::charcoal);
		renderer.canvas.Begin(worldCamera.GetViewProjection());

			for (Mover& mover: movers) 
			{
				mover.Attract(attractorPosition);
				mover.Update(deltaTime);
				mover.Draw(renderer);
			}

			renderer.canvas.DrawCircle(Matrix3::CreateTranslation(attractorPosition), 10, 6, Vector4::white);

		renderer.canvas.End();
	}

	Vector2 GetInputAxis() 
	{
		Vector2 direction;

		if (window.GetInput<Keyboard>().IsKeyDown(Key::Left))
			direction.x -= 1;

		if (window.GetInput<Keyboard>().IsKeyDown(Key::Right))
			direction.x += 1;

		if (window.GetInput<Keyboard>().IsKeyDown(Key::Up))
			direction.y -= 1;

		if (window.GetInput<Keyboard>().IsKeyDown(Key::Down))
			direction.y += 1;

		return Vector2::Normalize(direction);
	};

private:
	CanvasCamera worldCamera;
	CanvasCamera uiCamera;
	std::shared_ptr<Font> font;
};