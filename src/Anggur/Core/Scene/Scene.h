#pragma once

#include "Anggur/Core/Process.h"
#include "Anggur/Common/Observer.h"
#include "Anggur/Graphics/Render/Renderer.h"
#include "Anggur/OS/Window.h"
#include "Anggur/OS/WindowObserver.h"
#include <vector>

namespace Anggur
{
	class Scene : public WindowObserver
	{
	public:
		virtual void Initialize();

		virtual void Update(float deltaTime);

		virtual void Draw();

	protected:
		Window* window;
		Renderer* renderer;
		Process* process;

		friend class Application;
	};
}
