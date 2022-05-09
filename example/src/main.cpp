#include <Anggur/Window/WindowManager.h>
#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Texture.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Math/Random.h>
#include <chrono>
#include "Renderer.h"
#include "ScreenVP.h"

using namespace Anggur;

float getElapsedTime() {
	static auto begin = std::chrono::steady_clock::now();
	auto current = std::chrono::steady_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(current - begin).count() * 0.001;
}

int main() {
	WindowManager wm;
	Window w(Vector2(400, 300), "Example Window");

	Renderer r;

	struct Rectangle {
		Vector2 position;
		Vector2 size;
	};
	Rectangle rectangle;
	rectangle.position = Vector2(0, 0);
	rectangle.size = Vector2(100, 100);

	while (w.isOpen()) {
		// logic
		rectangle.position.x = Math::sin(getElapsedTime()) * 100;

		// rendering 
		r.begin();
	
		r.setViewProjection(createScreenVP(w.getSize()));
		r.clear();
	
		r.renderRectangle(rectangle.position, rectangle.size);
		r.end();

		w.swapBuffers();
		wm.pollEvents();
	}
}
