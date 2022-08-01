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
		camera.SetTargetPosition(Vector2(100, -100));
	
		font = std::make_shared<Font>();
        font->SetSample("resources/fonts/OpenSans-Regular.ttf", 128, 512);
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

			// for (size_t i = 0; i < font->glyphBuffers.size(); ++i) 
			// {
			// 	int space = 200;
			// 	auto& texture = font->glyphBuffers[i].texture;
			// 	renderer.canvas.DrawTexturedRectangle(Matrix3(), Vector2(0, i * space), space, texture, 0, 1);
			// }

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