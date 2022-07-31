#pragma once

#include "BaseApp.h"
#include "Anggur/Graphics/CanvasCamera.h"
#include "Anggur/Graphics/Font.h"
#include <iostream>

using namespace Anggur;

class App: public BaseApp 
{
public:
	void Initialize() 
	{
		camera.SetFrameSize(window.GetSize());
		camera.SetFrameScale(0.5);
		// camera.SetTargetPosition(Vector2(1, 1));
	
		font = std::make_shared<Font>();
        font->SetFont("resources/fonts/OpenSans-Regular.ttf", 128, 512);
        font->GenerateASCII();
	}

	void Update(float deltaTime) 
	{
		if (window.input.IsKeyDown(Key::LeftShift)) 
			camera.NudgeFrameRotation(deltaTime * GetInputAxis().x);
		else
			camera.NudgeTargetPosition(deltaTime * GetInputAxis() * 100);
		

		camera.NudgeFrameScale(deltaTime * window.input.GetScrollDirection().y);

		camera.Update();
		
		renderer.canvas.Begin(camera.GetViewProjection());

			renderer.canvas.Clear(Vector4::charcoal);

			TextOptions textOptions;
			textOptions.size = 64;
			textOptions.letterSpacing = 1;
			textOptions.wordSpacing = 10;
			renderer.canvas.DrawText(Matrix3(), "Hello darkness my old friend. I've come to talk with you again", font, textOptions);

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
	std::shared_ptr<Font> font;
};