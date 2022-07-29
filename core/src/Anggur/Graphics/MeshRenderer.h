#pragma once

#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector3.h"
#include "Anggur/Math/Vector4.h"
#include "Anggur/Math/Matrix4.h"
#include "Texture2D.h"
#include <vector>
#include <memory>

namespace Anggur 
{
	struct MeshVertex 
	{
		Vector3 position;
		Vector3 normal;
		Vector4 color;
		Vector2 uv;
		float slot;

		MeshVertex() = default;
		MeshVertex(const Vector3& newPosition, const Vector3& newNormal, const Vector4& newColor, const Vector2& newUV, float newSlot = 0.0f): 
			position(newPosition), normal(newNormal), color(newColor), uv(newUV), slot(newSlot) {
		}

		std::string toString() {
			std::string temp;

			temp += "position: " + position.toString() + "\n";
			temp += "normal: " + normal.toString() + "\n";
			temp += "color: " + color.ToString() + "\n";
			temp += "textureCoord: " + uv.toString() + "\n";
			temp += "textureSlot: " + std::to_string(slot) + "\n"; 

			return temp;
		}
	};

	class MeshRenderer 
	{
	public:
		MeshRenderer();

		~MeshRenderer();

		void setBatchChunk(size_t vertex, size_t indexMultiplier = 2);
		void clear(const Vector4& color = Vector4::black);
		void setViewport(const Vector2& size);
		void setViewport(const Vector2& position, const Vector2& size);
		void setViewProjection(const Matrix4& newViewProjection);

		void begin();
		void begin(const Matrix4& viewProjection);
		void end();

		bool isCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize);
		void flush();
		void flushInternalBuffer();

		void draw(const std::vector<MeshVertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture);

		// 3D Primitives
		void drawCube(const Matrix4& model, const Vector4& color = Vector4::white);

	private:
        std::shared_ptr<Shader> shader;
        std::shared_ptr<VertexArray> vertexArray;
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<IndexBuffer> indexBuffer;

        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<std::shared_ptr<Texture2D>> textures;
        std::vector<int> textureSlots;

        std::shared_ptr<Texture2D> whiteTexture;

        size_t vertexOffset = 0;
        size_t indexOffset = 0;
        size_t textureOffset = 0;

        size_t renderCount = 0;
        size_t batchVertex = 128;
        size_t batchIndexMultiplier = 2;

        Matrix4 viewProjection;

	private:
		void InitializeVertexPool();
		void InitializeTexturePool();
		void InitializeShader();
	};
}