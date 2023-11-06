#pragma once

#include "anggur/os/input.h"
#include "anggur/os/windowobserver.h"
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
		unsigned int width;
		unsigned int height;

		unsigned int frameBufferWidth;
		unsigned int frameBufferHeight;

		std::string title;

		WindowContext* context;
		WindowObserver* observer;

		Input input;
	};
}
