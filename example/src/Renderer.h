#pragma once

#include "Anggur/Utility/Log.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector3.h"
#include "Anggur/Math/Vector4.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Transform.h"
#include <vector>
#include <memory>

namespace Anggur 
{
	class Renderer 
	{
	public:
		struct Vertex 
		{
			Vector3 position;
			Vector3 normal;
			Vector4 color;
			Vector2 textureCoord;
			float textureSlot;

			Vertex() = default;
			Vertex(const Vector3& newPosition, const Vector3& newNormal, const Vector4& newColor, const Vector2& newTextureCoord, float newTextureSlot = 0.0f): 
				position(newPosition), normal(newNormal), color(newColor), textureCoord(newTextureCoord), textureSlot(newTextureSlot) 
			{
			}
		};

		static void Initialize();

		static void SetBatchChunk(size_t vertex, size_t indexMultiplier = 2);
		static void Clear(const Vector4& color = Vector4::black);
		static void SetViewport(const Vector2& size);
		static void SetViewport(const Vector2& position, const Vector2& size);
		static void SetViewProjection(const Matrix4& newViewProjection);

		static void Begin();
		static void Begin(const Matrix4& viewProjection);
		static void End();

		static bool IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize);
		static void Flush();
		static void FlushData();

		static void Render(const std::vector<Vertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture);
		static void RenderRectangle(const Transform& transform, const std::shared_ptr<Texture2D>& texture, const Vector2& texturePosition = Vector2::zero, const Vector2& textureSize = Vector2::one, const Vector4& color = Vector4::white);

		static void RenderPolygon(const Vector2& position, int segment, float length, float angle, const Vector4& color = Vector4::white);
		static void RenderCircle(const Vector2& position, float length, const Vector4& color = Vector4::white);

	private:
		Renderer();

		static void InitializeVertexPool();
		static void InitializeTexturePool();
		static void InitializeShader();
	};
}