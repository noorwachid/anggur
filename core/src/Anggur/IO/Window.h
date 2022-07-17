#pragma once

#include "Anggur/EventManager.h"
#include "WindowEvent.h"

struct GLFWwindow;

namespace Anggur 
{
	using WindowContext = GLFWwindow;

	class Window 
	{
	public:
		EventManager eventManager;

	public:
		Window(const Vector2& size = Vector2(800, 600), const std::string& title = "");
		
		~Window();


		WindowContext* GetContext();

		float GetAspectRatio();

		const Vector2& GetPosition();

		const Vector2& GetSize();

		const Vector2& GetFrameBufferSize();

		const std::string& GetTitle();

		void SetPosition(const Vector2& pos);
		
		void SetSize(const Vector2& size);
		
		void SetTitle(const std::string& title);

		bool IsOpen();

		void Close();
		
		void Bind();
		
		void SwapBuffers();

	private:
		void InitializeGraphics();
		void BindContext();

	private:
		WindowContext* context;
		Vector2 position;
		Vector2 size;
		Vector2 frameBufferSize;
		std::string title;
	};
}
