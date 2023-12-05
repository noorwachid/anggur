#pragma once

#include "Anggur/OS/Input.h"
#include "Anggur/OS/WindowObserver.h"
#include <string>

class GLFWwindow;

namespace Anggur
{
	using WindowContext = GLFWwindow;

	class Window
	{
	public:
		Window(unsigned int width, unsigned int height, const std::string& title);

		~Window();

		unsigned int GetWidth() const;

		unsigned int GetHeight() const;

		unsigned int GetFrameBufferWidth() const;

		unsigned int GetFrameBufferHeight() const;

		const std::string GetTitle() const;

		WindowContext* GetContext();

		Input* GetInput();

		bool ShouldClose() const;

		void SwapBuffers();

		void PollEvents();

		void Connect(std::function<bool(void*)> connector);

		void Close();

		void SetObserver(WindowObserver* observer);

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
