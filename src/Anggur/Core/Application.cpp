#include <SDL.h>
#include <SDL_events.h>
#include <SDL_timer.h>
#include <glad/glad.h>
#include <Anggur/Helper/Log.h>
#include <Anggur/Renderer/Renderer.h>
#include "Application.h"
#include "Input.h"

namespace Anggur
{
	WindowConfig Application::_windowConfig;

	Window* Application::_window = nullptr;

	Scene* Application::_currentScene = nullptr;

	float Application::_deltaTime = 0;

	float Application::_elapsedTime = 0;

	Application::Application()
	{
	}

	Window* Application::GetWindow()
	{
		return _window;
	}

	void Application::SetScene(Scene* activeScene)
	{
		if (_currentScene)
		{
			_currentScene->OnDestroy();
			delete _currentScene;
			_currentScene = nullptr;
		}
		_currentScene = activeScene;
		if (_currentScene)
		{
			_currentScene->OnCreate();
		}
	}

	float Application::GetDeltaTime()
	{
		return _deltaTime;
	}

	float Application::GetElapsedTime()
	{
		return _elapsedTime;
	}

	void Application::ProcessEvent(SDL_Event* event)
	{
		switch (event->type)
		{
			case SDL_QUIT:
			{
				_window->Close();
				break;
			}
			case SDL_WINDOWEVENT:
			{
				switch (event->window.event)
				{
					case SDL_WINDOWEVENT_MOVED:
					{
						break;
					}
					case SDL_WINDOWEVENT_RESIZED:
					{
						break;
					}
				}
				break;
			}
			case SDL_KEYDOWN:
			{
				KeyEvent e;
				e.type = EventType::KeyPressed;
				e.scancode = static_cast<Scancode>(event->key.keysym.scancode),
				e.key = static_cast<Key>(event->key.keysym.sym),
				e.modifier = static_cast<Modifier>(event->key.keysym.mod);
				if (_currentScene) _currentScene->OnEvent(e);
				break;
			}
			case SDL_KEYUP:
			{
				KeyEvent e;
				e.type = EventType::KeyReleased;
				e.scancode = static_cast<Scancode>(event->key.keysym.scancode),
				e.key = static_cast<Key>(event->key.keysym.sym),
				e.modifier = static_cast<Modifier>(event->key.keysym.mod);
				if (_currentScene) _currentScene->OnEvent(e);
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				MouseScrollEvent e;
				e.type = EventType::MouseScrolled;
				e.group = EventGroup::Mouse;
				e.direction.Set(event->wheel.x, event->wheel.y);
				Input::_wheel = e.direction;
				if (_currentScene) _currentScene->OnEvent(e);
				break;
			}
			case SDL_MOUSEMOTION:
			{
				MouseMovementEvent e;
				e.type = EventType::MouseMoved;
				e.group = EventGroup::Mouse;
				e.position.Set(event->motion.x, event->motion.y);
				e.deltaPosition.Set(event->motion.xrel, event->motion.yrel);
				Input::_cursor = e.position;
				if (_currentScene) _currentScene->OnEvent(e);
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				MouseButtonEvent e;
				e.type = EventType::MouseButtonPressed;
				e.group = EventGroup::Mouse;
				e.position.Set(event->button.x, event->button.y);
				e.button = static_cast<MouseButton>(event->button.button);
				if (_currentScene) _currentScene->OnEvent(e);
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				MouseButtonEvent e;
				e.type = EventType::MouseButtonReleased;
				e.group = EventGroup::Mouse;
				e.position.Set(event->button.x, event->button.y);
				e.button = static_cast<MouseButton>(event->button.button);
				if (_currentScene) _currentScene->OnEvent(e);
				break;
			}
			default:
				break;
		}
	}

	void Application::Run(Scene* mainScene)
	{
		Application::Initialize();

		_window = new Window(_windowConfig);
		Input::_windowHandler = _window->_windowHandler;

		Renderer::Initialize();
		Input::Initialize();

		SDL_Event event;
		ulong prevTimePoint = SDL_GetPerformanceCounter();

		_currentScene = mainScene;
		if (_currentScene)
		{
			_currentScene->OnCreate();
		}

		while (_window->IsOpen())
		{
			Input::PreUpdate();

			while (SDL_PollEvent(&event))
				ProcessEvent(&event);

			ulong currTimePoint = SDL_GetPerformanceCounter();
			_deltaTime = (currTimePoint - prevTimePoint) / static_cast<float>(SDL_GetPerformanceFrequency());
			_elapsedTime += _deltaTime;
			prevTimePoint = currTimePoint;

			Input::Update();
			if (_currentScene)
			{
				_currentScene->OnUpdate();
			}

			_window->SwapBuffers();
		}


		if (_currentScene)
		{
			_currentScene->OnDestroy();
			delete _currentScene;
			_currentScene = nullptr;
		}

		Renderer::Terminate();
		Terminate();

	}

	void Application::SetWindowConfig(const WindowConfig& config)
	{
		_windowConfig = config;
	}

	void Application::SetVsync(bool enable)
	{
		SDL_GL_SetSwapInterval(enable);
	}

	void Application::Initialize()
	{
		static bool isInitialized = false;

		if (!isInitialized)
		{
			// Do not disable compositing
			SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

			int result = SDL_Init(SDL_INIT_VIDEO);
			ANGGUR_ASSERT(result == 0, "[Core] Failed to initialize SDL: %i\n", result);

			isInitialized = true;
		}
	}

	void Application::Terminate()
	{
		SDL_Quit();
	}

	void Application::LoadGraphicsFunctions()
	{
		static bool isInitialized = false;

		if (!isInitialized)
		{
			int result = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
			ANGGUR_ASSERT(result != 0, "[Core] Failed to load OpenGl functions");
			isInitialized = true;
		}

		// Anggur_Log("[Core] Loaded OpenGl functions\n");
	}
}
