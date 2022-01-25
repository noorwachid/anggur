#include <string.h>
#include <SDL_events.h>
#include <Anggur/Helper/Log.h>
#include "Input.h"

namespace Anggur
{
	SDL_Window* Input::_windowHandler = nullptr;

	const uint8_t* Input::_currentScancodeMap = nullptr;

	uint8_t Input::_previousScancodeMap[ANGGUR_SCANCODE_SIZE];

	uint Input::_currentButtonMap = 0;

	uint Input::_previousButtonMap = 0;

	Vector Input::_cursor;

	Vector Input::_wheel;

	void Input::Initialize()
	{
		_currentScancodeMap = SDL_GetKeyboardState(nullptr);
		memset(_previousScancodeMap, 0, ANGGUR_SCANCODE_SIZE);
	}

	void Input::PreUpdate()
	{
		memcpy(_previousScancodeMap, _currentScancodeMap, ANGGUR_SCANCODE_SIZE);

		_previousButtonMap = _currentButtonMap;
		_wheel.Set(0, 0);
	}

	void Input::Update()
	{
		int x, y;
		_currentButtonMap = SDL_GetMouseState(&x, &y);
		_cursor = Vector(x, y);
	}

	bool Input::IsKeyPressed(Scancode scancode)
	{
		int i = static_cast<int>(scancode);
		return _currentScancodeMap[i] == 1 && _previousScancodeMap[i] == 0;
	}

	bool Input::IsKeyHeld(Scancode scancode)
	{
		int i = static_cast<int>(scancode);
		return _currentScancodeMap[i] && _previousScancodeMap[i];
	}

	bool Input::IsKeyReleased(Scancode scancode)
	{
		int i = static_cast<int>(scancode);
		return !_currentScancodeMap[i] && _previousScancodeMap[i];
	}

	bool Input::IsKeyPressed(Key key)
	{
		int i = SDL_GetScancodeFromKey(static_cast<int>(key));
		return _currentScancodeMap[i] == 1 && _previousScancodeMap[i] == 0;
	}

	bool Input::IsKeyHeld(Key key)
	{
		int i = SDL_GetScancodeFromKey(static_cast<int>(key));
		return _currentScancodeMap[i] && _previousScancodeMap[i];
	}

	bool Input::IsKeyReleased(Key key)
	{
		int i = SDL_GetScancodeFromKey(static_cast<int>(key));
		return !_currentScancodeMap[i] && _previousScancodeMap[i];
	}

	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		Uint32 btn = static_cast<Uint32>(button);
		return !(_previousButtonMap & btn) && (_currentButtonMap & btn);
	}

	bool Input::IsMouseButtonHeld(MouseButton button)
	{
		Uint32 btn = static_cast<Uint32>(button);
		return (_previousButtonMap & btn) && (_currentButtonMap & btn);
	}

	bool Input::IsMouseButtonReleased(MouseButton button)
	{
		Uint32 btn = static_cast<Uint32>(button);
		return (_previousButtonMap & btn) && !(_currentButtonMap & btn);
	}

	void Input::SetMouseCursor(const Vector& pos)
	{
		int x = pos.x;
		int y = pos.y;
		SDL_WarpMouseInWindow(_windowHandler, x, y);
	}

	Vector Input::GetMouseCursor()
	{
		return _cursor;
	}

	Vector Input::GetMouseWheel()
	{
		return _wheel;
	}
}

