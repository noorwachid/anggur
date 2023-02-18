#pragma once

#include "Input.h"
#include "WindowContext.h"
#include "WindowEvent.h"

namespace Anggur
{
	class Window
	{
	  public:
        Keyboard keyboard;
        Mouse mouse;

	  public:
		Window(const Vector2& size = Vector2(800, 600), const std::string& title = "");

		~Window();

		/**
		 * Get native context
		 */
		WindowContext* GetContext();

		/**
		 * Get aspect ratio
		 */
		float GetAspectRatio();

		/**
		 * Get cursor position
		 */
		const Vector2& GetCursorPosition();

		/**
		 * Get size
		 */
		const Vector2& GetSize();

		const Vector2& GetFrameBufferSize();

		const std::string& GetTitle();

		void SetCursorPosition(const Vector2& pos);

		void SetSize(const Vector2& size);

		void SetTitle(const std::string& title);

		bool IsOpen();

		void Close();

		void Update();

		template <class InputDevice> InputDevice& GetInputDevice()
		{
			static InputDevice device(*this);

			return device;
		}

        void Dispatch(Event& event)
        {
            if (listener)
                listener(event);
        }

        void SetListener(const std::function<void(Event&)>& newListener)
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
		Vector2 frameBufferSize;
		std::string title;

        std::function<void(Event&)> listener;
	};
}
