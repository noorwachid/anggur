#pragma once

#include "Anggur/OS/Input.h"
#include "Anggur/OS/WindowObserver.h"
#include <string>

class GLFWwindow;

namespace Anggur {
	using WindowContext = GLFWwindow;

	class Window {
	public:
		Window(unsigned int width, unsigned int height, const std::string& title);

		~Window();

		unsigned int getWidth() const;

		unsigned int getHeight() const;

		unsigned int getFrameBufferWidth() const;

		unsigned int getFrameBufferHeight() const;

		const std::string getTitle() const;

		WindowContext* getContext();

		Input* getInput();

		bool shouldClose() const;

		void swapBuffers();

		void pollEvents();

		void connect(std::function<bool(void*)> connector);

		void close();

		void setObserver(WindowObserver* observer);

	private:
		unsigned int _width;
		unsigned int _height;

		unsigned int _frameBufferWidth;
		unsigned int _frameBufferHeight;

		std::string _title;

		WindowContext* _context;
		WindowObserver* _observer;

		Input _input;
	};
}
