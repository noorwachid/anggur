#pragma once

#include <Anggur/Math/Vector2.h>
#include <string>
#include <vector>

struct GLFWwindow;

namespace Anggur {
	using WindowHandler = GLFWwindow;

	class Window {
	public:
		Window(const Vector2& size = Vector2(800, 600), const std::string& title = "");

		virtual ~Window();

		void setPosition(const Vector2& pos);
		void setSize(const Vector2& size);
		void setTitle(const std::string& title);

		float getAspectRatio();
		const Vector2& getPosition();
		const Vector2& getSize();
		const Vector2& getBufferSize();
		const std::string& getTitle();
		WindowHandler* getHandler();

		bool isOpen();
		void close();
		void bind();
		
		void swapBuffers();

	private:
		void initializeGraphicsFunctions();
		void initializeEventEmmiter();

	private:
		WindowHandler* handler = nullptr;
		std::string title;

		Vector2 position;
		Vector2 size;
		Vector2 bufferSize;
	};
}
