#include <Anggur/System/Window.h>
#include <Anggur/System/Input.h>
#include <iostream>
#include "Renderer.h"
#include "VP2D.h"

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

int main() 
{
	using namespace Anggur;

	App::Initialize(Vector2(400, 300));

	Renderer::SetViewProjection(CreateScreenVP(Window::GetSize()));

	while (App::IsOpen()) 
	{
		App::BeginFrame();

		Renderer::Begin();

			Renderer::Clear();
			
			Renderer::RenderRectangle(Vector2(100, 100), 50, Vector4::red);

		Renderer::End();

		App::EndFrame();
	}
}