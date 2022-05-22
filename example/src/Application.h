#pragma once

#include <Anggur/Window/WindowManager.h>
#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Texture2D.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/System/File.h>
#include <Anggur/System/Memory.h>
#include "Renderer2D.h"
#include "ScreenVP.h"
#include <msdf-atlas-gen/msdf-atlas-gen.h>

using namespace std::chrono_literals;

struct Application 
{
	struct Rectangle 
	{
		Vector2 position;
		Vector2 size;
	};

	Application() 
	{
		WindowManager::Initialize();
		WindowManager::Add(CreateShared<Window>(Vector2(400, 300)));

		Renderer2D::Initialize();

		auto texture = CreateShared<Texture2D>("resources/images/wall.jpeg");

		while (WindowManager::IsOpen())
		{
			Renderer2D::SetViewProjection(CreateScreenVP(WindowManager::At(0)->GetSize()));
			Renderer2D::SetClearColor(Vector4::charcoal);

			Renderer2D::Begin();

				Renderer2D::Clear();

				Renderer2D::RenderRectangle(Vector2(0.0f, 0.0f), Vector2(200.0f, 200.0f), texture, Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f), Vector4::white);

			Renderer2D::End();
			
			WindowManager::PollEvents();
			WindowManager::SwapBuffers();
		}
	}
};
