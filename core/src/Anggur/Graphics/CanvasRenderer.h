#pragma once

#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include "Anggur/Math/Matrix3.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "Texture2D.h"
#include <vector>
#include <memory>

namespace Anggur 
{
	struct CanvasVertex 
	{
		Vector2 position;
		Vector4 color;
		Vector2 uv;
		float slot;

		CanvasVertex() = default;
		CanvasVertex(const Vector2& newPosition, const Vector4& newColor, const Vector2& newUV, float newSlot = 0.0f): 
			position(newPosition), color(newColor), uv(newUV), slot(newSlot) 
		{
		}

		std::string ToString() 
		{
			std::string temp;

			temp += "position: " + position.ToString() + "\n";
			temp += "color: " + color.ToString() + "\n";
			temp += "uv: " + uv.ToString() + "\n";
			temp += "slot: " + std::to_string(slot) + "\n"; 

			return temp;
		}
	};

	class CanvasRenderer 
	{
	public:
        CanvasRenderer();

        ~CanvasRenderer();

		void SetBatchChunk(size_t vertex, size_t indexMultiplier = 2);
		void Clear(const Vector4& color = Vector4::black);
		void SetViewport(const Vector2& size);
		void SetViewport(const Vector2& position, const Vector2& size);
		void SetViewProjection(const Matrix3& newViewProjection);

		void Begin();
		void Begin(const Matrix3& viewProjection);
		void End();

		bool IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize);
		void Flush();
		void FlushInternalBuffer();

		void Draw(const std::vector<CanvasVertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture);

		// 2D Primitives
		void DrawTriangle(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& point2, const Vector4& color = Vector4::white);
		void DrawQuad(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& point2, const Vector2& point3, const Vector4& color = Vector4::white);
		void DrawRectangle(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector4& color = Vector4::white);
		
        void DrawTexturedRectangle(const Matrix3& model, const Vector2& point0, const Vector2& point1, const std::shared_ptr<Texture2D>& texture, const Vector2& texturePoint0 = Vector2::zero, const Vector2& texturePoint1 = Vector2::one, const Vector4& color = Vector4::white);

        // 2D Circles
		void DrawArc(const Matrix3& model, float radius = 1.0f, float beginAngle = 0.0f, float sweepAngle = Math::pi, int segment = 16, const Vector4& color = Vector4::white);
		void DrawCircle(const Matrix3& model, float radius = 1.0f, int segment = 32, const Vector4& color = Vector4::white);

		// 2D Lines
		void DrawLineTerminator(const Matrix3& model, const Vector2& point0, const Vector2& point1, float thickness = 0.5, const Vector4& color = Vector4::white);
		void DrawLineAnchor(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& p2, float thickness = 0.5, const Vector4& color = Vector4::white);
		void DrawLine(const Matrix3& model, const Vector2& point0, const Vector2& point1, float thickness = 0.5, const Vector4& color = Vector4::white);
		void DrawPolyLine(const Matrix3& model, const std::vector<Vector2>& points, float thickness = 0.5, const Vector4& color = Vector4::white);
		void DrawClosedPolyLine(const Matrix3& model, const std::vector<Vector2>& points, float thickness = 0.5, const Vector4& color = Vector4::white);

		// 2D Texts
		enum class TextAlign 
		{
			Left,
			Center,
			Right,
			Justify,
		};

		void DrawText(const Matrix3& model, const Vector2& point0, const Vector2& point1, const std::string& textBuffer, TextAlign textVerticalAlign, TextAlign textHorizontalAlign, const Vector4& color = Vector4::white);

    private:
        std::shared_ptr<Shader> shader;
        std::shared_ptr<VertexArray> vertexArray;
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<IndexBuffer> indexBuffer;

        std::vector<CanvasVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<std::shared_ptr<Texture2D>> textures;
        std::vector<int> slots;

        std::shared_ptr<Texture2D> whiteTexture;

        size_t vertexOffset = 0;
        size_t indexOffset = 0;
        size_t textureOffset = 0;

        size_t renderCount = 0;
        size_t batchVertex = 128;
        size_t batchIndexMultiplier = 2;

        Matrix3 viewProjection;

	private:
		void InitializeVertexPool();
		void InitializeTexturePool();
		void InitializeShader();

	};
}