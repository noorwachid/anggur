#include "Anggur/IO/WindowSystem.h"
#include "Anggur/IO/Window.h"
#include "Anggur/IO/Clock.h"
#include "Anggur/IO/Internal.h"
#include "Anggur/Graphics/CanvasRenderer.h"
#include "Anggur/Graphics/CanvasCamera.h"
#include "Anggur/Graphics/MeshRenderer.h"

int main() 
{
	using namespace Anggur;

	WindowSystem windowSystem;
	Window window(Vector2(600, 600), "An Empty Window");
	Clock clock;

	CanvasRenderer renderer;
	CanvasCamera camera;
	Matrix3 model;

	Vector2 p0 = {-1, -0.5};
	Vector2 p1 = {1, 1};

	auto letterFTexture = std::make_shared<Texture2D>("resources/images/f.png");

	std::cout << window.getSize().toString() << "\n";

	window.emmiter.listen("frameBufferResized", [&renderer](Event& event) {
		auto frameBufferEvent = static_cast<FrameBufferSizeEvent&>(event);
		renderer.setViewport(frameBufferEvent.size);
	});

	camera.setFrameSize(window.getSize());
	camera.setFrameScale(0.5);

	window.emmiter.listen("windowResized", [&camera](Event& event) {
		auto windowEvent = static_cast<WindowSizeEvent&>(event);
		camera.setFrameSize(windowEvent.size);
	});

	Vector2 ballPosition;
	float ballVelocity = 30;
	float previousTime = clock.getElapsed();

	auto getWindowClose = [&]() -> void {
		if (window.input.isKeyPressed(Key::escape))
			window.close();
	};

	auto getInputAxis = [&]() -> Vector2 {
		Vector2 direction;

		if (window.input.isKeyDown(Key::left))
			direction.x -= 1;

		if (window.input.isKeyDown(Key::right))
			direction.x += 1;

		if (window.input.isKeyDown(Key::up))
			direction.y -= 1;

		if (window.input.isKeyDown(Key::down))
			direction.y += 1;

		return Vector2::normalize(direction);
	};

	while (window.isOpen()) {
		windowSystem.pollEvents();

		float currentTime = clock.getElapsed();
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		ballPosition += ballVelocity * deltaTime * getInputAxis();

		camera.update();
		
		renderer.begin(camera.getViewProjection());

		renderer.clear(Vector4::charcoal);

		renderer.drawTexturedRectangle(Matrix3::createTranslation(ballPosition), {0, 0}, {20, 20}, letterFTexture, {0, 0}, {1, 1});

		renderer.end();

		window.update();
	}
}