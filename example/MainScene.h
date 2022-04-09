#pragma once

#include "Anggur/Core/Application.h"
#include "Anggur/Graphic/Renderer.h"

using Anggur::Scene;
using Anggur::Renderer;
using Anggur::Vector4;

class MainScene: public Scene {
	void onUpdate() {
		Renderer::clear(Vector4::red);
	}
};
