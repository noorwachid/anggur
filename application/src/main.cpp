#include "Anggur/IO/WindowSystem.h"
#include "Anggur/IO/Window.h"
#include "Anggur/IO/Internal.h"
#include "Anggur/Graphics/CanvasRenderer.h"
#include "Anggur/Graphics/MeshRenderer.h"

int main() 
{
	using namespace Anggur;

	WindowSystem windowSystem;
	Window window(Vector2(600, 600), "An Empty Window");

	CanvasRenderer renderer;
	Matrix3 model;

	Vector2 p0 = {-1, -0.5};
	Vector2 p1 = {1, 1};

	while (window.isOpen())
	{
		windowSystem.pollEvents();

		if (window.input.isKeyPressed(Key::escape))
			window.close();

		renderer.begin();

		renderer.clear(Vector4::charcoal);

		renderer.drawLine(model, {0, 0}, {0.5, 0.25}, 0.1);

		// renderer.drawQuad(model, {-0.5, -0.5}, {0.5, -0.5}, {0.5, 0.5}, {-0.5, 0.5});

		renderer.end();

		window.update();
	}
}