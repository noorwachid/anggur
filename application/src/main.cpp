#include "Anggur/IO/WindowSystem.h"
#include "Anggur/IO/Window.h"
#include "Anggur/Graphics/CanvasRenderer.h"
#include "Anggur/Graphics/MeshRenderer.h"

int main() 
{
	using namespace Anggur;

	WindowSystem windowSystem;
	Window window(Vector2(600, 400), "An Empty Window");

	// CanvasRenderer renderer;
	MeshRenderer renderer;

	while (window.isOpen())
	{
		windowSystem.pollEvents();

		if (window.input.isKeyPressed(Key::escape))
			window.close();

		renderer.begin();

		renderer.clear(Vector4::charcoal);

		float rotation = Math::map(window.input.getMousePosition().x, 0, window.getSize().x, 0, Math::twoPi);

		renderer.drawCube(Matrix4::createScale(0.3));

		renderer.end();

		window.update();
	}
}