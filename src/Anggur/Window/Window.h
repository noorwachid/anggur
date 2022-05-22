#pragma once

#include <Anggur/Window/Event/Common.h>
#include <Anggur/System/String.h>
#include <Anggur/System/Array.h>

struct GLFWwindow;

namespace Anggur 
{
	using WindowHandler = GLFWwindow;

	class Window 
	{
	public:
		Window(const Vector2& size = Vector2(800, 600), const String& title = "");

		virtual ~Window();

		void SetPosition(const Vector2& pos);
		void SetSize(const Vector2& size);
		void SetTitle(const String& title);

		float GetAspectRatio();
		const Vector2& GetPosition();
		const Vector2& GetSize();
		const String& GetTitle();
		WindowHandler* GetHandler();

		bool IsOpen();
		void Close();
		void Bind();
		
		void SwapBuffers();

	private:
		void InitializeGraphicsFunctions();
		void InitializeEventEmmiter();
		void EmitEvent(Event& event);

	private:
		WindowHandler* handler = nullptr;
		String title;

		Vector2 size;
		Vector2 position;
	};
}
