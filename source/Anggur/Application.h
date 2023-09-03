#pragma once

#include "Anggur/Scene.h"
#include "Anggur/OS/Window.h"
#include "Anggur/Graphics/Renderer.h"

namespace Anggur
{
	class Application
	{
	public:
		Application(uint32_t width, uint32_t height);

		~Application();

		template <class Scene>
		void SetScene()
		{
			if (_scene)
				delete _scene;

			_scene = new Scene();
			_scene->_window = &_window;
			_scene->_renderer = _renderer;
			_scene->Initialize();
		}


		void Run();

	private:
		Window _window;
		Renderer* _renderer;
		Scene* _scene;
	};
}
