#pragma once

#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Texture2D.h>
#include <Anggur/Graphics/FrameBuffer.h>
#include <Anggur/Window/WindowManager.h>
#include <Anggur/Window/Internal.h>
#include <Anggur/Scene/Scene.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/System/File.h>
#include "Renderer.h"
#include "VP2D.h"

namespace Anggur {
	struct FirstScene: public Anggur::Scene {
		void initialize() {
		}

		void update() {
			Renderer::begin(Matrix4());

			Renderer::renderCube();

			Renderer::end();
		}

	public:
		std::shared_ptr<Texture2D> texture;
	};
}