#pragma once

#include "Anggur/Core/Application.h"
#include "Anggur/Core/Log.h"
#include "Anggur/Graphic/Renderer.h"

using namespace Anggur;

class View: public Entity {
	void initialize() {
	}

	void render() {
		Renderer::drawCircle(transform.toMatrix3(), 0.1, 0.1, Vector4::red);
	}
};

class MainScene: public Scene {
	void initialize() {
		children.push_back(new View());
	}

	void render() {
		Renderer::clear(Vector4::charcoal);
	}
};
