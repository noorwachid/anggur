#pragma once

#include "Anggur/Event/EventManager.h"
#include "WindowEvent.h"

namespace Anggur 
{
	class Window 
	{
	public:
		static void Initialize(const Vector2& size = Vector2(800, 600), const std::string& title = "");
		static void SetPosition(const Vector2& pos);
		static void SetSize(const Vector2& size);
		static void SetTitle(const std::string& title);

		static void* GetHandler();

		static float GetAspectRatio();
		static const Vector2& GetPosition();
		static const Vector2& GetSize();
		static const Vector2& GetFrameBufferSize();
		static const std::string& GetTitle();
		
		static bool IsOpen();
		static void Close();
		static void Bind();
		static void SwapBuffers();
		static void PollEvents();

		static EventManager& GetEventManager();

	private:
		static void InitializeGraphicsFunctions();
	};
}
