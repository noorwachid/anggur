#include <string.h>
#include "Log.h"
#include "Input.h"

namespace Anggur {
	void Input::Initialize() {
	}

	void Input::PreUpdate() {
	}

	void Input::Update() {
	}

	bool Input::IsKeyPressed(Scancode scancode) {
		return true;
	}

	bool Input::IsKeyHeld(Scancode scancode) {
		return true;
	}

	bool Input::IsKeyReleased(Scancode scancode) {
		return true;
	}

	bool Input::IsKeyPressed(Key key) {
		return true;
	}

	bool Input::IsKeyHeld(Key key) {
		return true;
	}

	bool Input::IsKeyReleased(Key key) {
		return true;
	}

	bool Input::IsMouseButtonPressed(MouseButton button) {
		return true;
	}

	bool Input::IsMouseButtonHeld(MouseButton button) {
		return true;
	}

	bool Input::IsMouseButtonReleased(MouseButton button) {
		return true;
	}

	void Input::SetMouseCursor(const Vector2& pos) {
	}

	Vector2 Input::GetMouseCursor() {
		return Vector2();
	}

	Vector2 Input::GetMouseWheel() {
		return Vector2();
	}
}

