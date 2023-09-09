#pragma once

#include "Anggur/OS/Window.h"
#include "Anggur/Graphics/Renderer.h"

namespace Anggur
{
	class Scene: public WindowObserver
	{
	public:
		virtual void Initialize() {}

		virtual void Update(float deltaTime) {}

		virtual void Terminate() {}

	protected:
		Window* _window;
		Renderer* _renderer;

		friend class Application;
	};
}
