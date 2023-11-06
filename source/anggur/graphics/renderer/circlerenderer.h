#pragma once

#include "anggur/graphics/font.h"
#include "anggur/graphics/shader.h"
#include "anggur/graphics/texture.h"
#include "anggur/graphics/vertexbuffer.h"
#include "anggur/math/matrix3.h"
#include "anggur/math/vector2.h"
#include "anggur/math/vector4.h"

namespace Anggur {
	struct CircleVertex {
		Vector2 position;
		Vector2 quadrant;
		float radius = 0.5f;
		float thickness = 0.5f;
		float sharpness = 0.01f;
		Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	};

	class CircleRenderer {
	public:
		CircleRenderer();

		void setView(const Matrix3& view);

		void add(const Vector2& position, float radius, float thickness, float sharpness, const Vector4& color);

		void flush();

	private:
		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		Shader shader;

		Matrix3 view;

		std::vector<CircleVertex> vertices;
		std::vector<unsigned int> indices;

		size_t vertexOffset = 0;
		size_t indexOffset = 0;

		size_t drawCount = 0;

		size_t batchVertex = 512;
		size_t batchIndexMultiplier = 2;
	};
}
