#pragma once

#include "BaseApp.h"
#include "Anggur/Graphics/CanvasCamera.h"
#include "Anggur/Graphics/Font.h"
#include "Anggur/Math/Random.h"
#include <iostream>

using namespace Anggur;

class App: public BaseApp {
public:
	void initialize() {
		camera.setScreenSize(window.getSize());
		camera.setViewSize(window.getSize());
		camera.setTargetPosition(Vector2(
			window.getSize().x / -2.0f,
			window.getSize().y / -2.0f
		));

		font = std::make_shared<Font>();
        font->setSample("resource/font/OpenSans-Regular.ttf", 64, 512);
        font->generateASCII();
	}

	void update(float deltaTime) {
		camera.Update();

		renderer.canvas.clear(Vector4::charcoal);
		renderer.canvas.begin(camera.getViewProjection());

			TextOptions textOptions;
			textOptions.boxSize = Vector2(100, 100);
			textOptions.size = 200;
			textOptions.horizontalWrap = true;

		    renderer.canvas.drawRectangle(Matrix3(), Vector2(0, 0), Vector2(100, 100), Vector4::chocolate);
		    renderer.canvas.drawText(Matrix3(), "Hello World!", font, textOptions);

		renderer.canvas.End();
	}

private:
	CanvasCamera camera;
	std::shared_ptr<Font> font;
};