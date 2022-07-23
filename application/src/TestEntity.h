#pragma once

#include "Entity.h"
#include "Anggur/Graphics/CanvasCamera.h"

using namespace Anggur;

class TestEntity: public Entity {
public:
	void initialize() {
		camera.setFrameSize(window->getSize());
		camera.setFrameScale(0.5);
	}

	void update(float deltaTime) {
		ballPosition += deltaTime * getInputAxis() * ballVelocity;

		camera.update();
		
		renderer->canvas.begin(camera.getViewProjection());

			renderer->canvas.clear(Vector4::charcoal);
			renderer->canvas.drawCircle(Matrix3::createTranslation(ballPosition), 10);

		renderer->canvas.end();
	}

	Vector2 getInputAxis() {
		Vector2 direction;

		if (window->input.isKeyDown(Key::left))
			direction.x -= 1;

		if (window->input.isKeyDown(Key::right))
			direction.x += 1;

		if (window->input.isKeyDown(Key::up))
			direction.y -= 1;

		if (window->input.isKeyDown(Key::down))
			direction.y += 1;

		return Vector2::normalize(direction);
	};

private:
	CanvasCamera camera;

	Vector2 ballPosition;
	Vector2 ballVelocity = 100;
};