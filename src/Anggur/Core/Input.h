#pragma once

#include "Anggur/Math/Vector2.h"
#include "Anggur/Core/Application.h"
#include "Anggur/Core/Event/Event.h"

struct SDL_Window;

namespace Anggur {
	class Input {
	public:
		static void Initialize();
		static void PreUpdate();
		static void Update();

		static bool IsKeyPressed(Scancode scancode);
		static bool IsKeyHeld(Scancode scancode);
		static bool IsKeyReleased(Scancode scancode);

		static bool IsKeyPressed(Key key);
		static bool IsKeyHeld(Key key);
		static bool IsKeyReleased(Key key);

		static bool IsMouseButtonPressed(MouseButton button);
		static bool IsMouseButtonHeld(MouseButton button);
		static bool IsMouseButtonReleased(MouseButton button);

		static void SetMouseCursor(const Vector2& pos);

		static Vector2 GetMouseCursor();
		static Vector2 GetMouseWheel();

		friend class Application;
	};
}

