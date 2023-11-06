#pragma once

#include "anggur/audio/audiodevice.h"
#include "anggur/graphics/renderer.h"
#include "anggur/os/window.h"
#include "anggur/scene.h"

namespace Anggur {
	class Application {
	public:
		Application(uint32_t width, uint32_t height, const std::string& title);

		~Application();

		template <class Scene>
		void setScene() {
			if (scene)
				delete scene;

			scene = new Scene();
			scene->window = &window;
			scene->input = window.getInput();
			scene->renderer = renderer;
			scene->initialize();
			window.setObserver(scene);
		}

		template <class Scene>
		Scene* getScene() {
			return static_cast<Scene*>(scene);
		}

		// No cap
		void run();

		// Same update as the framerate
		void stableRun();

		// Only update on demand
		void lazyRun();

	private:
		Window window;
		Renderer* renderer;
		Scene* scene;
		AudioDevice audioDevice;
	};
}
