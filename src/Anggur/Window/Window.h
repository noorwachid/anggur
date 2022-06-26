#pragma once

#include <Anggur/Math/Vector2.h>
#include <Anggur/Event/EventManager.h>
#include <string>
#include <vector>

namespace Anggur 
{
	struct WindowSizeEvent: public Event
	{
		Vector2 size;

		WindowSizeEvent(const std::string& name, const Vector2& newSize): Event(name), size(newSize)
		{
		}
	};

	struct WindowPositionEvent: public Event
	{
		Vector2 position;

		WindowPositionEvent(const std::string& name, const Vector2& newPosition): Event(name), position(newPosition)
		{
		}
	};

	struct FrameBufferSizeEvent: public Event
	{
		Vector2 size;

		FrameBufferSizeEvent(const std::string& name, const Vector2& newSize): Event(name), size(newSize)
		{
		}
	};

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
