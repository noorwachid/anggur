#pragma once

#include "Anggur/Audio/AudioDevice.h"
#include "Anggur/Graphics/Renderer.h"
#include "Anggur/OS/Window.h"
#include "Anggur/Scene.h"

namespace Anggur {
	class Application {
	public:
		Application(uint32_t width, uint32_t height, const std::string& title);

		~Application();

		template <class Scene>
		void setScene() {
			if (_scene)
				delete _scene;

			_scene = new Scene();
			_scene->_window = &_window;
			_scene->_input = _window.getInput();
			_scene->_renderer = _renderer;
			_scene->initialize();
			_window.setObserver(_scene);
		}

		template <class Scene>
		Scene* getScene() {
			return static_cast<Scene*>(_scene);
		}

		// No cap
		void run();

		// Same update as the framerate
		void stableRun();

		// Only update on demand
		void lazyRun();

	private:
		Window _window;
		Renderer* _renderer;
		Scene* _scene;
		AudioDevice _audioDevice;
	};
}
