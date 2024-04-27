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

		void SetScene(Scene* scene);

		// No cap
		void Run();

		// Same update as the framerate
		void StableRun();

	private:
		Window _window;
		Renderer* _renderer;
		Scene* _scene;
		AudioDevice _audioDevice;
	};
}
