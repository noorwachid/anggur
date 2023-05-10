#pragma once

#include "Anggur/OS/Input.h"
#include "Anggur/OS/WindowContext.h"
#include "Anggur/OS/WindowObserver.h"
#include <functional>

namespace Anggur
{
	using WindowContextLoader = std::function<bool (void*)>;

	class Window
	{
	public:
        Input input;

	public:
		Window(const Vector2& size, const std::string& title, const WindowContextLoader& contextLoader);

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

        void SetObserver(WindowObserver* observer)
        {
            this->observer = observer;
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

        WindowObserver* observer = nullptr;

		friend class Input;
	};
}
