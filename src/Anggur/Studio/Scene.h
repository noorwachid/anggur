#pragma once

#include "Anggur/Core/Observer.h"
#include "Anggur/Core/Process.h"
#include "Anggur/Graphics/Render/Renderer.h"
#include "Anggur/OS/Window.h"
#include "Anggur/OS/WindowObserver.h"
#include <vector>

namespace Anggur {
	class Scene : public WindowObserver {
	public:
		virtual void initialize();

		virtual void update(float deltaTime);

		virtual void draw();

	protected:
		Window* window;
		Renderer* renderer;
		Process* process;

		friend class Application;
	};
}
