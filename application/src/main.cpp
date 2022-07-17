#include "Anggur/IO/WindowSystem.h"
#include "Anggur/IO/Window.h"
#include "Anggur/Graphics/CanvasRenderer.h"

int main() 
{
	using namespace Anggur;

	WindowSystem windowSystem;
	Window window(Vector2(600, 400), "An Empty Window");

	CanvasRenderer renderer;

	while (window.isOpen())
	{
		windowSystem.pollEvents();

		if (window.input.isKeyPressed(Key::Escape))
			window.close();

		renderer.Begin();

		renderer.Clear(Vector4::charcoal);

		renderer.DrawRectangle(Matrix3(), {0, 0}, {0.5, 0.5});

		renderer.end();

		window.update();
	}
}