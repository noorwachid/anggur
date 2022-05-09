#pragma once

#include <string>
#include <vector>
#include <functional>
#include <Anggur/Window/Event/Common.h>
#include <Anggur/Math/Vector2.h>

struct GLFWwindow;

namespace Anggur {
	using WindowHandler = GLFWwindow;
	using EventListener = std::function<void (Event&)>;

	class Window {
		// TODO: Implement subwindow

	public:
		Window(const Vector2& size = Vector2(800, 600), const std::string& title = "");

		~Window();

		void setPosition(const Vector2& pos);
		void setSize(const Vector2& size);
		void setTitle(const std::string& title);

		float getAspectRatio();
		const Vector2& getPosition();
		const Vector2& getSize();
		const std::string& getTitle();
		WindowHandler* getHandler();

		bool isOpen();
		void close();
		void bind();
		
		void swapBuffers();

		void addEventListener(EventListener eventListener);
		void removeEventListener(EventListener eventListener);

	private:
		void initializeGraphicsFunctions();
		void initializeEventEmmiter();
		void emitEvent(Event& event);

	private:
		WindowHandler* handler = nullptr;
		std::vector<EventListener> eventListeners;
		std::string title;

		Vector2 size;
		Vector2 position;
	};
}
