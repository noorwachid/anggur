#include <SDL.h>
#include <Anggur/Math/Vector.h>
#include <Anggur/Renderer/Image.h>
#include <Anggur/Helper/IO.h>
#include <Anggur/Helper/Log.h>
#include "Window.h"
#include "Application.h"

namespace Anggur
{
	Window::Window(const WindowConfig& config)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		int flag = SDL_WINDOW_OPENGL | static_cast<int>(config.flag);

		_windowHandler = SDL_CreateWindow(config.title.c_str(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			config.width, config.height,
			flag);
		_context = SDL_GL_CreateContext(_windowHandler);
		_ratio = config.width / static_cast<float>(config.height);
		_open = true;
		SDL_GL_MakeCurrent(_windowHandler, _context);

		Application::LoadGraphicsFunctions();
	}

	void Window::SetPosition(const Vector& pos)
	{
		SDL_SetWindowPosition(_windowHandler, pos.x, pos.y);
	}

	void Window::SetSize(const Vector& size)
	{
		SDL_SetWindowSize(_windowHandler, size.x, size.y);
	}

	void Window::SetTitle(const string& title)
	{
		SDL_SetWindowTitle(_windowHandler, title.c_str());
	}

	float Window::GetRatio()
	{
		return _ratio;
	}

	Vector Window::GetPosition()
	{
		int x, y;
		SDL_GetWindowPosition(_windowHandler, &x, &y);
		return Vector(x, y);
	}

	Vector Window::GetSize()
	{
		int w, h;
		SDL_GetWindowSize(_windowHandler, &w, &h);
		return Vector(w, h);
	}

	const string& Window::GetTitle()
	{
		_title = SDL_GetWindowTitle(_windowHandler);
		return _title;
	}

	Anggur::WindowFlag Window::GetFlag()
	{
		return static_cast<WindowFlag>(SDL_GetWindowFlags(_windowHandler));
	}

	bool Window::IsOpen()
	{
		return _open;
	}

	void Window::SwapBuffers()
	{
		SDL_GL_SwapWindow(_windowHandler);
	}

	SDL_Window* Window::GetHandler()
	{
		return _windowHandler;
	}

	void* Window::GetContext()
	{
		return _context;
	}

	void Window::Close()
	{
		_open = false;
	}
}
