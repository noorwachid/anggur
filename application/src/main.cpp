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

		if (window.input.isKeyPressed(Key::escape))
			window.close();

		renderer.begin();

		renderer.clear(Vector4::charcoal);

		renderer.drawRectangle(Matrix3(), {0, 0}, {0.5, 0.5});

		renderer.end();

		window.update();
	}
}