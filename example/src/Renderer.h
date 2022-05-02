#pragma once

#include <Anggur/Core/Application.h>
#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Texture.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Math/Vector2.h>
#include <Anggur/Math/Vector4.h>
#include <Anggur/Math/Matrix3.h>
#include <memory>

using namespace Anggur;

class Renderer {
public:
	struct Vertex {
		Vector2 position;
		Vector4 color;
		Vector2 texCoord;
		float texSlot;

		Vertex() = default;
		Vertex(const Vector2& newPosition, const Vector4& newColor, const Vector2& newTexCoord, float newTexSlot): 
			position(newPosition), color(newColor), texCoord(newTexCoord), texSlot(newTexSlot) {
		}
	};

	static void initialize();
	static void setClearColor(const Vector4& color);
	static void setBatchChunk(size_t vertex, size_t indexMultiplier = 2);
	static void clear();

	static void setViewProjection(const Matrix3& newViewProjection);

	static void beginScene();
	static void endScene();

	static void flush();

	static void render(const std::vector<Vertex>& newVertices, const std::vector<uint32_t>& newIndices);

	static void renderRectangle(const Vector2& position, const Vector2& size, const Vector4& color = Vector4::white);

private:
	static void initializeVertexArray();
	static void initializeShader();

private:
	static std::unique_ptr<Shader> shader;
	static std::unique_ptr<VertexArray> vertexArray;
	static std::unique_ptr<VertexBuffer> vertexBuffer;
	static std::unique_ptr<IndexBuffer> indexBuffer;
	static std::vector<Vertex> vertices;
	static std::vector<uint32_t> indices;

	static size_t vertexOffset;
	static size_t indexOffset;
	static size_t renderCount;
	static size_t batchVertex;
	static size_t batchIndexMultiplier;

	static Matrix3 viewProjection;
};