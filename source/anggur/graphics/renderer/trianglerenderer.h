#pragma once

#include "anggur/graphics/font.h"
#include "anggur/graphics/shader.h"
#include "anggur/graphics/texture.h"
#include "anggur/graphics/vertexbuffer.h"
#include "anggur/math/matrix3.h"
#include "anggur/math/vector2.h"
#include "anggur/math/vector4.h"

namespace Anggur {
	struct TriangleVertex {
		Vector2 position;
		Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
		float textureIndex;
		Vector2 texturePosition;
	};

	class TriangleRenderer {
	public:
		TriangleRenderer();

		void setView(const Matrix3& view);

		void addTriangle(
			const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color,
			Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
			const Vector2& texturePosition2
		);

		void addQuad(
			const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3,
			const Vector4& color, Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
			const Vector2& texturePosition2, const Vector2& texturePosition3
		);

		void addRectangle(
			const Vector2& position, const Vector2& size, const Vector4& color, Texture* texture,
			const Vector2& texturePosition, const Vector2& textureSize
		);

		void flush();

	private:
		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		Shader shader;

		Matrix3 view;

		std::vector<TriangleVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture*> textures;
		std::vector<int> textureIndices;

		std::unordered_map<unsigned int, size_t> textureIndexMap;

		size_t previousVertexOffset = 0;
		size_t previousIndexOffset = 0;
		size_t vertexOffset = 0;
		size_t indexOffset = 0;
		size_t textureOffset = 0;

		size_t drawCount = 0;

		size_t batchVertex = 512;
		size_t batchIndexMultiplier = 2;
	};
}
