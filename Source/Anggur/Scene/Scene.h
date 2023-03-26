#pragma once

#include "Anggur/Event.h"
#include "Anggur/Graphics/Render/Renderer.h"
#include "Anggur/IO/Window.h"
#include "Anggur/IO/WindowEvent.h"
#include <vector>

namespace Anggur
{
	class Scene : public NotificationEventListener, public WindowEventListener
	{
	public:
		virtual void Initialize();

		virtual void Update(float deltaTime);

		virtual void Draw();

		void SetWindow(Window* newWindow);

		void SetRenderer(Renderer* newRenderer);

	protected:
		Window* window = nullptr;
		Renderer* renderer = nullptr;
	};
}
