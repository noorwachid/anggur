#pragma once

#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Math/Matrix4.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector3.h"
#include "Anggur/Math/Vector4.h"
#include "Anggur/Render/Mesh.h"
#include <memory>
#include <vector>

namespace Anggur
{
	class MeshRenderer
	{
	public:
		MeshRenderer();

		~MeshRenderer();

		void SetBatchChunk(size_t vertex, size_t indexMultiplier = 2);
		void Clear(const Vector4& color = Vector4::black);
		void SetViewport(const Vector2& size);
		void SetViewport(const Vector2& position, const Vector2& size);
		void SetViewProjection(const Matrix4& newViewProjection);

		void BeginScene();
		void BeginScene(const Matrix4& viewProjection);
		void EndScene();

		bool IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize);
		void Flush();
		void FlushInternalBuffer();

		inline const std::shared_ptr<Texture2D>& GetDefaultTexture() const
		{
			return whiteTexture;
		}

		void Draw(const Matrix4& model, const Mesh& mesh);

	private:
		std::shared_ptr<Shader> shader;
		std::shared_ptr<VertexArray> vertexArray;
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<std::shared_ptr<Texture2D>> textures;
		std::vector<int> textureSlots;

		std::shared_ptr<Texture2D> whiteTexture;

		size_t vertexOffset = 0;
		size_t indexOffset = 0;
		size_t textureOffset = 0;

		size_t drawCount = 0;
		size_t batchVertex = 128;
		size_t batchIndexMultiplier = 2;

		Matrix4 viewProjection;

	private:
		void InitializeVertexPool();
		void InitializeTexturePool();
		void InitializeShader();
	};
}
