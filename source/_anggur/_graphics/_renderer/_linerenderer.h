#pragma once

#include "Anggur/Graphics/Font.h"
#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture.h"
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
		VertexArray _vertexArray;
		VertexBuffer _vertexBuffer;
		IndexBuffer _indexBuffer;

		Shader _shader;

		Matrix3 _view;

		std::vector<LineVertex> _vertices;
		std::vector<unsigned int> _indices;

		size_t _vertexOffset = 0;
		size_t _indexOffset = 0;

		size_t _drawCount = 0;

		size_t _batchVertex = 512;
		size_t _batchIndexMultiplier = 2;
	};
}
