#pragma once

#include "BaseApp.h"
#include "Anggur/Graphics/CanvasCamera.h"
#include "Anggur/Graphics/Font.h"
#include <iostream>

using namespace Anggur;

class App: public BaseApp 
{
public:
	std::vector<std::shared_ptr<Texture2D>> textures;
	TextSystem textSystem;

	void Initialize() 
	{
		camera.SetFrameSize(window.GetSize());
		camera.SetFrameScale(0.5);
		// camera.SetTargetPosition(Vector2(1, 1));
        
        textSystem.SetFont("resources/fonts/OpenSans-Regular.ttf", 128);
        textSystem.GenerateASCII();

		for (auto buffer: textSystem.glyphBuffers)
			textures.push_back(std::make_shared<Texture2D>(buffer.data.data(), buffer.width, buffer.height, 1));
	}

	void Update(float deltaTime) 
	{
		if (window.input.IsKeyDown(Key::LeftShift)) 
			camera.NudgeFrameRotation(deltaTime * GetInputAxis().x);
		else
			camera.NudgeTargetPosition(deltaTime * GetInputAxis());
		

		camera.NudgeFrameScale(deltaTime * window.input.GetScrollDirection().y);

		camera.Update();
		
		renderer.canvas.Begin(camera.GetViewProjection());

			renderer.canvas.Clear(Vector4::charcoal);

			for (size_t i = 0; i < textures.size(); ++i) {

				renderer.canvas.DrawTexturedRectangle(Matrix3(), Vector2(0, 200 * i), 200, textures[i], 0, 1);
			}

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
};