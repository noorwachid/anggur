#pragma once

#include "Anggur/Graphics/Renderer.h"
#include "Anggur/OS/Window.h"

namespace Anggur
{
	class Scene : public WindowObserver
	{
	public:
		virtual void Initialize()
		{
		}

		virtual void Update(float deltaTime)
		{
		}

		virtual void Terminate()
		{
		}

	protected:
		Window* _window;
		Input* _input;
		Renderer* _renderer;

		friend class Application;
	};
}
