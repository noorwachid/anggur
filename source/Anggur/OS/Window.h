#pragma once

#include <string>

class GLFWwindow;

namespace Anggur
{
	using WindowContext = GLFWwindow;

	class Window
	{
	public:
		Window(uint32_t width, uint32_t height);

		~Window();

		uint32_t GetWidth() const;

		uint32_t GetHeight() const;

		WindowContext* GetContext();

		bool ShouldClose() const;

		void SwapBuffers();

		void PollEvents();

		void Connect(std::function<bool(void*)> client);

		void Close();

	private:
		uint32_t _width;
		uint32_t _height;

		WindowContext* _context;
	};
}
