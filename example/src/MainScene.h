#pragma once

#include "Anggur/Core/Application.h"
#include "Anggur/Graphic/Renderer.h"

using namespace Anggur;

class MainScene: public Scene {
	Camera camera;
	Font font;
	TextOption textOption;
	std::string textBuffer;

	void initialize() {
		font.load("resource/font/OpenSans-Regular.ttf");
		textOption.size = 24;
		textBuffer = "ABC";

		usePixelMapping();
	}

	void usePixelMapping() {
		Vector2 windowSize = Application::getWindow()->getSize();
		camera.viewport = windowSize;
		camera.distance = 1.0f / (windowSize.x * 0.5f);
		camera.offset.x = (windowSize.x * -0.5f);
		camera.offset.y = (windowSize.y * -0.5f);

		Renderer::setViewProjection(camera.toMatrix3());
	}

	void render() {
		Renderer::clear(Vector4::charcoal);

		Renderer::drawText(Matrix3::identity, Vector2(0, 0), Vector2(50, 50), textBuffer, textOption, font);
	}

	void onEvent(Event& event) {
		if (event.type == EventType::codepointInput) {
			auto e = static_cast<CodepointEvent&>(event);
			textBuffer += e.codepoint;
		}
	}
};
