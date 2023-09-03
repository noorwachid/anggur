#include "Anggur/Application.h"
#include "Anggur/Graphics/Pointer.h"

namespace Anggur
{
	Application::Application(uint32_t width, uint32_t height) : _window(width, height), _scene(nullptr)
	{
		_window.Connect([](void* address) -> bool { return ConnectPointer(address); });
		_renderer = new Renderer();

	}

	Application::~Application()
	{
		if (_scene) delete _scene;

		delete _renderer;
	}

	void Application::Run()
	{
		while (!_window.ShouldClose())
		{
			if (_scene) _scene->Update(0.016f);

			_window.SwapBuffers();
			_window.PollEvents();
		}
	}
}
