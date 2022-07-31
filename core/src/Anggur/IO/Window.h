#pragma once

#include "Anggur/EventEmitter.h"
#include "WindowEvent.h"
#include "WindowContext.h"
#include "Input.h"

namespace Anggur 
{
	class Window 
	{
	public:
		EventEmitter emmiter;
		Input input;

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

		void Update();

	private:
		void InitializeGraphics();

		void BindGraphics();

		void BindNativeContext();

		void SwapFrameBuffers();

	private:
		WindowContext* context;
		Vector2 position;
		Vector2 size;
		Vector2 frameBufferSize;
		std::string title;
	};
}
