#pragma once

#include "Anggur/OS/Window.h"
#include "Anggur/Graphics/Renderer.h"

namespace Anggur
{
	class Scene
	{
	public:
		virtual void Initialize() {}

		virtual void Update(float deltaTime) {}

	protected:
		Window* _window;
		Renderer* _renderer;

		friend class Application;
	};
}
