#pragma once

#include "Anggur/Graphics/Function.h"
#include "Anggur/Graphics/Render/Font.h"
#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"

namespace Anggur {
	struct LineVertex {
		Vector2 position;
		Vector2 positionA;
		Vector2 positionB;
		Vector2 quadrant;
		float thickness = 0.5f;
		float sharpness = 0.01f;
		Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	};

	class LineRenderer {
	public:
		LineRenderer();

		void setView(const Matrix3& newView);

		void add(
			const Vector2& positionA, const Vector2& positionB, float thickness, float sharpness, const Vector4& color
		);

		void flush();

	private:
		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		Shader shader;

		Matrix3 view;

		std::vector<LineVertex> vertices;
		std::vector<unsigned int> indices;

		size_t vertexOffset = 0;
		size_t indexOffset = 0;

		size_t drawCount = 0;

		size_t batchVertex = 512;
		size_t batchIndexMultiplier = 2;
	};
}
