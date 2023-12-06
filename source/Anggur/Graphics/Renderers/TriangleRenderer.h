#pragma once

#include "Anggur/Graphics/Font.h"
#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"

namespace Anggur
{
	struct TriangleVertex
	{
		Vector2 position;
		Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
		float textureIndex;
		Vector2 texturePosition;
	};

	class TriangleRenderer
	{
	public:
		TriangleRenderer();

		void SetView(const Matrix3& view);

		void AddTriangle(
			const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color,
			Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
			const Vector2& texturePosition2
		);

		void AddQuad(
			const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3,
			const Vector4& color, Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
			const Vector2& texturePosition2, const Vector2& texturePosition3
		);

		void AddRectangle(
			const Vector2& position, const Vector2& size, const Vector4& color, Texture* texture,
			const Vector2& texturePosition, const Vector2& textureSize
		);

		void Flush();

	private:
		VertexArray _vertexArray;
		VertexBuffer _vertexBuffer;
		IndexBuffer _indexBuffer;

		Shader _shader;

		Matrix3 _view;

		std::vector<TriangleVertex> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<Texture*> _textures;
		std::vector<int> _textureIndices;

		std::unordered_map<unsigned int, size_t> _textureIndexMap;

		size_t _previousVertexOffset = 0;
		size_t _previousIndexOffset = 0;
		size_t _vertexOffset = 0;
		size_t _indexOffset = 0;
		size_t _textureOffset = 0;

		size_t _drawCount = 0;

		size_t _batchVertex = 512;
		size_t _batchIndexMultiplier = 2;
	};
}
