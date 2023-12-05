#pragma once

#include "anggur/graphics/font.h"
#include "anggur/graphics/shader.h"
#include "anggur/graphics/texture.h"
#include "anggur/graphics/vertexbuffer.h"
#include "anggur/math/matrix3.h"
#include "anggur/math/vector2.h"
#include "anggur/math/vector4.h"

namespace Anggur
{
	struct LineVertex
	{
		Vector2 position;
		Vector2 positionA;
		Vector2 positionB;
		Vector2 quadrant;
		float thickness = 0.5f;
		float sharpness = 0.01f;
		Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	};

	class LineRenderer
	{
	public:
		LineRenderer();

		void SetView(const Matrix3& view);

		void Add(
			const Vector2& positionA, const Vector2& positionB, float thickness, float sharpness, const Vector4& color
		);

		void Flush();

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
