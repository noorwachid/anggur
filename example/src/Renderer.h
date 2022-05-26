#pragma once

#include <Anggur/Utility/Log.h>
#include <Anggur/Math/Vector2.h>
#include <Anggur/Math/Vector3.h>
#include <Anggur/Math/Vector4.h>
#include <Anggur/Math/Matrix4.h>
#include <Anggur/Graphics/Texture2D.h>
#include <vector>
#include <memory>

using namespace Anggur;

class Renderer {
public:
	struct Vertex {
		Vector3 position;
		Vector4 color;
		Vector2 textureCoord;
		float textureSlot;

		Vertex() = default;
		Vertex(const Vector3& newPosition, const Vector4& newColor, const Vector2& newTextureCoord, float newTextureSlot = 0.0f): 
			position(newPosition), color(newColor), textureCoord(newTextureCoord), textureSlot(newTextureSlot) {
		}
	};

	static void initialize();

	static void setBatchChunk(size_t vertex, size_t indexMultiplier = 2);
	static void clear(const Vector4& color = Vector4::black);

	static void setViewport(const Vector2& position, const Vector2& size);

	static void begin();
	static void begin(const Matrix4& newViewProjection);
	static void end();

	static bool isCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize);
	static void flush();
	static void flushData();

	static void render(const std::vector<Vertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture);
	static void renderRectangle(const Vector2& position, const Vector2& size, const Vector4& color = Vector4::white);

    static void renderCube();

private:
	Renderer();

	static void initializeVertexPool();
	static void initializeTexturePool();
	static void initializeShader();
};