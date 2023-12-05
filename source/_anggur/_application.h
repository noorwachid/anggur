#pragma once

#include "anggur/audio/audiodevice.h"
#include "anggur/graphics/renderer.h"
#include "anggur/os/window.h"
#include "anggur/scene.h"

namespace Anggur
{
	class Application
	{
	public:
		Application(uint32_t width, uint32_t height, const std::string& title);

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
