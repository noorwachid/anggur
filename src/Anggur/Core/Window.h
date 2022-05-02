#pragma once

#include <string>
#include <functional>
#include <Anggur/Core/Event/Common.h>
#include <Anggur/Math/Vector2.h>

struct GLFWwindow;

namespace Anggur {
	using WindowHandler = GLFWwindow;
	using EventCallback = std::function<void (Event&)>;

	class Window {
		// TODO: Implement subwindow

	public:
		Window(const Vector2& size = Vector2(800, 600), const std::string& title = "");

		~Window();

		void setPosition(const Vector2& pos);
		void setSize(const Vector2& size);
		void setTitle(const std::string& title);
		void setEventCallback(const EventCallback& callback);

		float getRatio();
		const Vector2& getPosition();
		const Vector2& getSize();
		const std::string& getTitle();
		WindowHandler* getHandler();

		bool isOpen();
		void close();

		void bind();
		void load();

		void swapBuffers();

		friend class Application;

		static void pollEvents();

	private:
		WindowHandler* handler;
		EventCallback eventCallback;
		std::string title;
	};
}
