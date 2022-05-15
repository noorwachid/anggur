#pragma once

#include <Anggur/Window/WindowManager.h>
#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Texture.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Math/Vector2.h>
#include <Anggur/Math/Vector4.h>
#include <Anggur/Math/Matrix3.h>

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

	Renderer();
	~Renderer();
	
	void setClearColor(const Vector4& color);
	void setBatchChunk(size_t vertex, size_t indexMultiplier = 2);
	void clear();

	void setViewProjection(const Matrix3& newViewProjection);

	void begin();
	void end();

	void flush();

	void render(const std::vector<Vertex>& newVertices, const std::vector<uint32_t>& newIndices);
	void renderRectangle(const Vector2& position, const Vector2& size, const Vector4& color = Vector4::white);
	void renderTexturedRectangle(const Vector2& position, const Vector2& size, const std::shared_ptr<Texture>& texture, const Vector2& texturePosition = Vector2::zero, const Vector2& textureSize = Vector2::one, const Vector4& color = Vector4::white);

private:
	void initializeVertexArray();
	void initializeShader();

private:
	Shader shader;
	VertexArray vertexArray;
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	size_t vertexOffset = 0;
	size_t indexOffset = 0;
	size_t textureOffset = 1;

	size_t renderCount = 0;
	size_t batchVertex = 128;
	size_t batchIndexMultiplier = 2;

	Matrix3 viewProjection;
	std::shared_ptr<Texture> blankTexture;
	std::vector<uint8_t> blankTextureData;
	std::vector<std::shared_ptr<Texture>> textures;
};
