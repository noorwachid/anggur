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
	
	static Matrix4 createPerspectiveProjection(float fovY, float width, float height, float near, float far) {
		float yScale = Math::cot(fovY / 2.0f);
		float xScale = yScale * height / width;
		
		return Matrix4({
			xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, far / (far - near), 1.0f,
			0.0f, 0.0f, -near * far / (far - near), 0.0f,
		});
	}

	static Matrix4 createOrthoProjection(float width, float height, float near, float far) {
		return Matrix4({ 
			2.0f / width, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / height, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (far - near), 0.0f,
			0.0f, 0.0f, near / (near - far), 1.0f
		});
	}

	struct FirstScene: public Anggur::Scene {
		Matrix4 projection;

		void initialize() {
			projection = createOrthoProjection(800, 600, -100, 10);
			ANGGUR_LOG("%s", projection.toString().c_str());
		}

		void update() {

			Renderer::begin(projection);

			Renderer::renderCube();

			Renderer::end();
		}

	public:
		std::shared_ptr<Texture2D> texture;
	};
}