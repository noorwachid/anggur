#pragma once

#include "Anggur/EventEmitter.h"
#include "WindowEvent.h"
#include "WindowContext.h"
#include "Input.h"

namespace Anggur {
	class Window {
	public:
		EventEmitter emmiter;
		Input input;

	public:
		Window(const Vector2& size = Vector2(800, 600), const std::string& title = "");
		
		~Window();

		WindowContext* getContext();

		float getAspectRatio();

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

	private:
		void initializeGraphics();

		void bindGraphics();

		void bindContext();

		void swapFrameBuffers();

	private:
		WindowContext* context;
		Vector2 position;
		Vector2 size;
		Vector2 frameBufferSize;
		std::string title;
	};
}
