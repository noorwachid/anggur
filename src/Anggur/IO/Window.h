#pragma once

#include "Input.h"
#include "WindowContext.h"
#include "WindowEvent.h"

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

		const Vector2& GetPosition();

		const Vector2& GetSize();

		const std::string& GetTitle();

		void SetPosition(const Vector2& pos);

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
		std::string title;

        std::function<void(Event&)> listener;
	};
}
