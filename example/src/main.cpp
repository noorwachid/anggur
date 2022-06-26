#include <Anggur/System/Window.h>
#include <Anggur/System/Input.h>
#include <iostream>
#include "Renderer.h"
#include "VP2D.h"

namespace Anggur
{
	class App 
	{
	public:
		static void Initialize(const Vector2& windowSize)
		{
			Window::Initialize(windowSize);
			Input::Initialize();

			Renderer::Initialize();
		}

		static bool IsOpen()
		{
			return Window::IsOpen();
		}

		static void BeginFrame() 
		{
			Window::PollEvents();
		}

		static void EndFrame()
		{
			Window::SwapBuffers();
			Input::Update();
		}
	};
}

int main() 
{
	using namespace Anggur;

	App::Initialize(Vector2(400, 300));

	std::shared_ptr<Texture2D> wallTexture = std::make_shared<Texture2D>("./resources/images/wall.jpeg");

	while (App::IsOpen()) 
	{
		App::BeginFrame();

		Renderer::Begin(CreateScreenVP(Window::GetSize()));

			Renderer::Clear();
			
			Renderer::RenderRectangle(Vector2(100, 100), 50);

		Renderer::End();

		App::EndFrame();
	}
}