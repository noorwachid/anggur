#pragma once

#include <Anggur/Window/WindowManager.h>
#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Texture2D.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Math/Vector2.h>
#include <Anggur/Math/Vector4.h>
#include <Anggur/Math/Matrix3.h>

using namespace Anggur;

class Renderer2D
{
public:
	struct Vertex 
	{
		Vector2 position;
		Vector4 color;
		Vector2 texCoord;
		float texSlot;

		Vertex() = default;
		Vertex(const Vector2& newPosition, const Vector4& newColor, const Vector2& newTexCoord, float newTexSlot = 0.0f): 
			position(newPosition), color(newColor), texCoord(newTexCoord), texSlot(newTexSlot) {
		}
	};

	Renderer2D();
	~Renderer2D();
	
	void SetClearColor(const Vector4& color);
	void SetBatchChunk(size_t vertex, size_t indexMultiplier = 2);
	void Clear();

	void SetViewProjection(const Matrix3& newViewProjection);

	void Begin();
	void End();

	bool IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize);
	void Flush();

	void Render(const std::vector<Vertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture);
	void RenderRectangle(const Vector2& position, const Vector2& size, const std::shared_ptr<Texture2D>& texture, const Vector2& texturePosition = Vector2::zero, const Vector2& textureSize = Vector2::one, const Vector4& color = Vector4::white);

private:
	void InitializeVertexPool();
	void InitializeTexturePool();
	void InitializeShader();

private:
	Shader shader;
	VertexArray vertexArray;
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<std::shared_ptr<Texture2D>> textures;

	size_t vertexOffset = 0;
	size_t indexOffset = 0;
	size_t textureOffset = 0;

	size_t renderCount = 0;
	size_t batchVertex = 128;
	size_t batchIndexMultiplier = 2;

	Matrix3 viewProjection;
};
