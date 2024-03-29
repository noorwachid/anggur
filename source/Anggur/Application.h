#pragma once

#include "Anggur/Audio/AudioDevice.h"
#include "Anggur/Graphics/Renderer.h"
#include "Anggur/OS/Window.h"
#include "Anggur/Scene.h"

namespace Anggur
{
	class Application
	{
	public:
		Application(unsigned int width, unsigned int height, const std::string& title);

		~Application();

		template <class Scene>
		void SetScene()
		{
			if (_scene)
				delete _scene;

			_scene = new Scene();
			_scene->_window = &_window;
			_scene->_input = _window.GetInput();
			_scene->_renderer = _renderer;
			_scene->Initialize();
			_window.SetObserver(_scene);
		}

		template <class Scene>
		Scene* GetScene()
		{
			return static_cast<Scene*>(_scene);
		}

		// No cap
		void Run();

		// Same update as the framerate
		void StableRun();

		// Only update on demand
		void LazyRun();

	private:
		Window _window;
		Renderer* _renderer;
		Scene* _scene;
		AudioDevice _audioDevice;
	};
}
