#include <Anggur/Window/Window.h>
#include <Anggur/Window/Input.h>
#include <iostream>
#include "Renderer2D.h"
#include "VP2D.h"

int main() 
{
	using namespace Anggur;

	Window::Initialize(Vector2(400, 300));

	while (Window::IsOpen()) 
	{
		Window::BeginFrame();

		if (Input::IsKeyPressed(Key::Space)) 
		{
			std::cout << "Space is pressed\n";
		}

		Window::EndFrame();
	}
}