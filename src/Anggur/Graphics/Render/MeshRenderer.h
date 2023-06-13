#pragma once

#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Graphics/Render/Font.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include "Anggur/Graphics/Function.h"

namespace Anggur
{
	struct MeshVertex 
	{
		Vector2 position;
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float textureIndex;
		Vector2 texturePosition;
	};

	class MeshRenderer
	{
	public:
		MeshRenderer(); 

		void SetView(const Matrix3& newView);

		void AddTriangle(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color, Texture2D* texture, const Vector2& texturePosition0, const Vector2& texturePosition1, const Vector2& texturePosition2);

		void AddQuad(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3, const Vector4& color, Texture2D* texture, const Vector2& texturePosition0, const Vector2& texturePosition1, const Vector2& texturePosition2, const Vector2& texturePosition3);

		void AddRectangle(const Vector2& position, const Vector2& size, const Vector4& color, Texture2D* texture, const Vector2& texturePosition, const Vector2& textureSize);

		void Flush();

	private:
		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		Shader shader;

		Matrix3 view;

		std::vector<MeshVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture2D*> textures;
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


