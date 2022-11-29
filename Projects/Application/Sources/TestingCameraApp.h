#pragma once

#include "BaseApp.h"
#include "Anggur/Graphics/CanvasCamera.h"
#include "Anggur/Graphics/Font.h"
#include "Anggur/Math/Random.h"
#include <iostream>

using namespace Anggur;

struct Mover {
	Vector2 position;
	Vector2 velocity;
	Vector4 color;
	float desireStrength;

	void attract(const Vector2& attractor) {
		Vector2 localVelocity =  Vector2::normalize(attractor - position);
		velocity += localVelocity;
		velocity.setLengthLimit(100);
	}

	void update(float deltaTime) {
		position += deltaTime * velocity;
	}

	void draw(Renderer& renderer) {
		Vector2 heading = Vector2::createPolar(desireStrength, velocity.getAngle());
		Vector2 leftWing = Vector2::createPolar(desireStrength, velocity.getAngle() - Math::pi * 0.70);
		Vector2 rightWing = Vector2::createPolar(desireStrength, velocity.getAngle() + Math::pi * 0.70);

		renderer.canvas.drawQuad(Matrix3::createTranslation(position), Vector2(0.0f, 0.0f), rightWing, heading, leftWing, color);
	}

	static Mover createRandom(const Vector2& spawnPosition = Vector2::zero) {
		Random random;

		float maxRadius = 100.0f;
		float maxVelocity = 100.0f;

		Mover mover;
		mover.position = spawnPosition + Vector2::createPolar(random.range(-maxRadius, maxRadius), random.getAngle());
		mover.velocity = Vector2::createPolar(random.range(Math::epsilon, maxVelocity), random.getAngle());
		Vector2 bendedVelocity = Vector2::normalize(mover.velocity.getPerpendicular());
		bendedVelocity.setLength(random.range(Math::epsilon, maxVelocity));
		mover.velocity += bendedVelocity;
		mover.color = Vector4::lerp(Vector4::createHex(0xD60270), Vector4::createHex(0x0038A8), random.getNormal());
		mover.desireStrength = random.range(0.1f, 10.0f);

		return mover;
	}
};

class TestingCameraApp: public BaseApp {
public:
	std::vector<Mover> movers;

	int moverCount = 10;

	Vector2 mouseDragBeginPoint;
	Vector2 mouseDragEndPoint;
	bool mouseDragging = false;


	Vector2 attractorPosition;

	float attractorAcceleration = 10.0f;
	float attractorMaxVelocity = 1000.0f;
	float attractorVelocity = 0.0f;

	void initialize() override {
		window.setTitle("TestingCameraApp");

		worldCamera.setScreenSize(window.getSize());
		worldCamera.setViewSize(0.25 * window.getSize());

		uiCamera.setScreenSize(window.getSize());
		uiCamera.setViewSize(window.getFrameBufferSize());
	
		font = std::make_shared<Font>();
        font->setSample("resource/font/OpenSans-Regular.ttf", 64, 512);
        font->generateASCII();
		
		for (int i = 0; i < moverCount; ++i) {
			movers.push_back(std::move(Mover::createRandom()));
		}
	}

	void update(float deltaTime) override {
		Keyboard& keyboard = window.getInputDevice<Keyboard>();
		Mouse& mouse = window.getInputDevice<Mouse>();

		if (keyboard.isKeyDown(Key::space) && mouse.isButtonPressed(MouseButton::f1)) {
			mouseDragging = true;
			mouseDragBeginPoint = worldCamera.toWorldPoint(mouse.getCursorPosition());
		}

		if (mouseDragging && (mouse.isButtonReleased(MouseButton::f1) || keyboard.isKeyReleased(Key::space))) {
			mouseDragging = false;
		}

		if (mouseDragging && mouse.isButtonDown(MouseButton::f1)) {
			mouseDragEndPoint = worldCamera.toWorldPoint(mouse.getCursorPosition());
			worldCamera.nudgeTargetPosition(mouseDragEndPoint - mouseDragBeginPoint);
		}

		if (keyboard.isKeyDown(Key::leftShift)) {
			attractorVelocity = Math::clamp(attractorVelocity + attractorAcceleration, 0.0f, attractorMaxVelocity);
		}

		if (keyboard.isKeyDown(Key::rightShift)) {
			attractorVelocity = Math::clamp(attractorVelocity - attractorAcceleration, 0.0f, attractorMaxVelocity);
		}

		if (keyboard.isKeyDown(Key::enter)) {
			movers.push_back(Mover::createRandom(attractorPosition));
		}


		attractorPosition += deltaTime * attractorVelocity * getInputDeviceAxis();

		worldCamera.nudgeViewScale(deltaTime * mouse.getWheelDirection().y);
		worldCamera.update();

		renderer.canvas.clear(Vector4::charcoal);
		renderer.canvas.begin(worldCamera.getViewProjection());

			for (Mover& mover: movers) {
				mover.attract(attractorPosition);
				mover.update(deltaTime);
				mover.draw(renderer);
			}

			renderer.canvas.drawCircle(Matrix3::createTranslation(attractorPosition), 10, 6, Vector4::white);

		renderer.canvas.End();
	}

	Vector2 getInputDeviceAxis() {
		Keyboard& keyboard = window.getInputDevice<Keyboard>();
		Vector2 direction;

		if (keyboard.isKeyDown(Key::left))
			direction.x -= 1;

		if (keyboard.isKeyDown(Key::right))
			direction.x += 1;

		if (keyboard.isKeyDown(Key::up))
			direction.y -= 1;

		if (keyboard.isKeyDown(Key::down))
			direction.y += 1;

		return Vector2::normalize(direction);
	};

private:
	CanvasCamera worldCamera;
	CanvasCamera uiCamera;
	std::shared_ptr<Font> font;
};