#pragma once

#include "Anggur/IO/Input.h"
#include "Anggur/IO/WindowContext.h"
#include "Anggur/IO/WindowEvent.h"

namespace Anggur
{
	class Window
	{
	public:
        Input input;

	public:
		Window(const Vector2& size = Vector2(800, 600), const std::string& title = "");

		~Window();

		WindowContext* GetContext();

		float GetAspectRatio();

		float GetScale();

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

        void SetListener(WindowEventListener* newListener)
        {
            listener = newListener;
        }

	private:
		void InitializeGraphicsAPI();

		void BindGraphicsAPI();

		void BindContext();

		void SwapFrameBuffers();

	private:
		WindowContext* context;
		Vector2 position;
		Vector2 size;
		std::string title;

		Vector2 frameBufferSize;

        WindowEventListener* listener = nullptr;

		friend class Input;
	};
}
