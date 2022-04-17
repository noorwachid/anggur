#pragma once

#include <string>
#include <Anggur/Math/Vector2.h>

struct GLFWwindow;

namespace Anggur {
	using WindowHandler = GLFWwindow;

	class Window {
		// TODO: Implement subwindow

	public:
		Window(const Vector2& size = Vector2(800, 600), const std::string& title = "");

		~Window();

		void setPosition(const Vector2& pos);
		void setSize(const Vector2& size);
		void setTitle(const std::string& title);

		float getRatio();
		Vector2 getPosition();
		Vector2 getSize();

		const std::string& getTitle();

		bool isOpen();
		void close();

		void bind();
		void load();

		void swapBuffers();

		WindowHandler* getHandler();

		friend class Application;

	private:
		WindowHandler* handler;
		Vector2 position;
		Vector2 size;
		std::string title;
	};
}
