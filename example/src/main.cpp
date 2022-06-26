#include <Anggur/Window/Window.h>
#include <Anggur/Window/Input.h>
#include <iostream>
#include "Renderer2D.h"
#include "VP2D.h"

class App 
{
public:
	static void Initialize(const Vector2& windowSize)
	{
		Window::Initialize(windowSize);
		Input::Initialize();

		Renderer2D::Initialize();
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

	Renderer2D::SetViewProjection(CreateScreenVP(Window::GetSize()));

	while (App::IsOpen()) 
	{
		App::BeginFrame();

		Renderer2D::Begin();

			Renderer2D::Clear();
			
			Renderer2D::RenderRectangle(Input::GetMousePosition() - Vector2(25, 25), Vector2(50, 50));

		Renderer2D::End();

		App::EndFrame();
	}
}