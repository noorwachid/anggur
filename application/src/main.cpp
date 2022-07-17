#include "Anggur/IO/WindowSystem.h"
#include "Anggur/IO/Window.h"
#include "Anggur/Graphics/CanvasRenderer.h"

int main() 
{
	using namespace Anggur;

	WindowSystem windowSystem;
	Window window(Vector2(600, 400), "An Empty Window");

	CanvasRenderer renderer;

	while (window.IsOpen())
	{
		windowSystem.pollEvents();

		if (window.input.IsKeyPressed(Key::Escape))
			window.Close();

		renderer.Begin();

		renderer.Clear(Vector4::charcoal);

		renderer.DrawRectangle(Matrix3(), {0, 0}, {0.5, 0.5});

		renderer.End();

		window.Update();
	}
}