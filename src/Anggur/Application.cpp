#include "Anggur/Application.h"
#include "Anggur/Graphics/Function.h"

namespace Anggur
{
	Application::Application()
	{
		window = new Window(Vector2(800, 600), "Anggur", [](void* procAddress)
		{
			return gladLoadGLLoader((GLADloadproc) procAddress);
		});

		renderer = new Renderer();
	}
}
