#pragma once

#include "Anggur/OS/Input.h"
#include "Anggur/OS/WindowContext.h"
#include "Anggur/OS/WindowObserver.h"
#include <functional>

namespace Anggur {
	using WindowContextLoader = std::function<bool(void*)>;

	class Window {
	public:
		Input input;

	public:
		Window(const Vector2& size, const std::string& title, const WindowContextLoader& contextLoader);

		~Window();

		WindowContext* getContext();

		float getAspectRatio();

		float getScale();

		const Vector2& getPosition();

		const Vector2& getSize();

		const Vector2& getFrameBufferSize();

		const std::string& getTitle();

		void setPosition(const Vector2& pos);

		void setSize(const Vector2& size);

		void setTitle(const std::string& title);

		bool isOpen();

		void close();

		void update();

		void setObserver(WindowObserver* observer) {
			this->observer = observer;
		}

	private:
		void initializeGraphicsAPI();

		void bindGraphicsAPI();

		void bindGraphicsContext();

		void swapFrameBuffers();

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
