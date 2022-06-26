#pragma once

#include <Anggur/Utility/Log.h>
#include <Anggur/Math/Vector2.h>
#include <Anggur/Math/Vector4.h>
#include <Anggur/Math/Matrix3.h>
#include <Anggur/Graphics/Texture2D.h>
#include <vector>
#include <memory>

using namespace Anggur;

class Renderer2D 
{
public:
	struct Vertex 
	{
		Vector2 position;
		Vector4 color;
		Vector2 textureCoord;
		float textureSlot;

		Vertex() = default;
		Vertex(const Vector2& newPosition, const Vector4& newColor, const Vector2& newTextureCoord, float newTextureSlot = 0.0f): 
			position(newPosition), color(newColor), textureCoord(newTextureCoord), textureSlot(newTextureSlot) {
		}
	};

	static void Initialize();

	static void SetBatchChunk(size_t vertex, size_t indexMultiplier = 2);
	static void Clear(const Vector4& color = Vector4::black);
	static void SetViewport(const Vector2& size);
	static void SetViewport(const Vector2& position, const Vector2& size);
	static void SetViewProjection(const Matrix3& newViewProjection);

	static void Begin();
	static void Begin(const Matrix3& viewProjection);
	static void End();

	static bool IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize);
	static void Flush();
	static void FlushData();

	static void Render(const std::vector<Vertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture);
	static void RenderRectangle(const Vector2& position, const Vector2& size, const Vector4& color = Vector4::white);
	static void RenderRectangle(const Vector2& position, const Vector2& size, const std::shared_ptr<Texture2D>& texture, const Vector2& texturePosition = Vector2::zero, const Vector2& textureSize = Vector2::one, const Vector4& color = Vector4::white);

private:
	Renderer2D();

	static void InitializeVertexPool();
	static void InitializeTexturePool();
	static void InitializeShader();
};