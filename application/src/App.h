#pragma once

#include "BaseApp.h"
#include "Anggur/Graphics/CanvasCamera.h"
#include <iostream>

using namespace Anggur;

class App: public BaseApp 
{
public:
	void Initialize() 
	{
		camera.SetFrameSize(window.GetSize());
		camera.SetFrameScale(0.5);
	}

	void Update(float deltaTime) 
	{
		ballPosition += deltaTime * ballVelocity * GetInputAxis();

		camera.Update();
		
		renderer.canvas.Begin(camera.GetViewProjection());

			renderer.canvas.Clear(Vector4::charcoal);
			renderer.canvas.DrawCircle(Matrix3::CreateTranslation(ballPosition), 10);

		renderer.canvas.End();
	}

	Vector2 GetInputAxis() 
	{
		Vector2 direction;

		if (window.input.IsKeyDown(Key::Left))
			direction.x -= 1;

		if (window.input.IsKeyDown(Key::Right))
			direction.x += 1;

		if (window.input.IsKeyDown(Key::Up))
			direction.y -= 1;

		if (window.input.IsKeyDown(Key::Down))
			direction.y += 1;

		return Vector2::Normalize(direction);
	};

private:
	CanvasCamera camera;

	Vector2 ballPosition;
	float ballVelocity = 200;
};