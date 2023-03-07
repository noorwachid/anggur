#pragma once

#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include "Anggur/Render/Font.h"
#include <memory>
#include <vector>

namespace Anggur
{
	struct CanvasVertex
	{
		Vector2 position;
		Vector4 color;
		Vector2 uv;
		float slot;

		CanvasVertex() = default;
		CanvasVertex(const Vector2& newPosition, const Vector4& newColor, const Vector2& newUV, float newSlot = 0.0f)
			: position(newPosition), color(newColor), uv(newUV), slot(newSlot)
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

	enum class TextAlignment
	{
		Beginning,
		Middle,
		End,

		InBetween,
		Even,
	};

	struct TextOptions
	{
		float size = 1.0f;

		// Spacing
		float letterSpacing = 0.0f;
		float wordSpacing = 0.01f;
		float lineSpacing = 0.01f;

		// Alignment
		TextAlignment horizontalAlignment = TextAlignment::Beginning;
		TextAlignment verticalAlignment = TextAlignment::Beginning;

		// Wrap
		bool horizontalWrap = false;
		bool verticalWrap = false;

		// Bounding
		Vector2 boxOffset = {0, 0};
		Vector2 boxSize = {1, 1};

		TextOptions() = default;
		TextOptions(float newSize) : size(newSize)
		{
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

		void BeginScene();
		void BeginScene(const Matrix3& viewProjection);
		void EndScene();

		bool IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize);
		void Flush();
		void FlushInternalBuffer();

		void Draw(
			const std::vector<CanvasVertex>& newVertices, const std::vector<uint32_t>& newIndices,
			const std::shared_ptr<Texture2D>& texture
		);

		// Digital Geometry
		void DrawTriangle(
			const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& point2,
			const Vector4& color = Vector4::white
		);
		void DrawQuad(
			const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& point2,
			const Vector2& point3, const Vector4& color = Vector4::white
		);
		void DrawRectangle(
			const Matrix3& model, const Vector2& point, const Vector2& size, const Vector4& color = Vector4::white
		);

		void DrawTexturedRectangle(
			const Matrix3& model, const Vector2& point, const Vector2& size, const std::shared_ptr<Texture2D>& texture,
			const Vector2& uvPoint = Vector2::zero, const Vector2& uvSize = Vector2::one,
			const Vector4& color = Vector4::white
		);

		// Augmented-Natural Geometry
		void DrawArc(
			const Matrix3& model, float radius = 1.0f, float beginAngle = 0.0f, float sweepAngle = Math::pi,
			int segment = 16, const Vector4& color = Vector4::white
		);
		void DrawCircle(
			const Matrix3& model, float radius = 1.0f, int segment = 32, const Vector4& color = Vector4::white
		);

		// Semi-Digital-Natural Geometry
		void DrawRoundedTriangle();
		void DrawRoundedQuad();
		void DrawRoundedRectangle();

		// Line Geometry
		void DrawLineTerminator(
			const Matrix3& model, const Vector2& point0, const Vector2& point1, float thickness = 0.5,
			const Vector4& color = Vector4::white
		);
		void DrawLineAnchor(
			const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& p2,
			float thickness = 0.5, const Vector4& color = Vector4::white
		);
		void DrawLine(
			const Matrix3& model, const Vector2& point0, const Vector2& point1, float thickness = 0.5,
			const Vector4& color = Vector4::white
		);
		void DrawPolyLine(
			const Matrix3& model, const std::vector<Vector2>& points, float thickness = 0.5,
			const Vector4& color = Vector4::white
		);
		void DrawClosedPolyLine(
			const Matrix3& model, const std::vector<Vector2>& points, float thickness = 0.5,
			const Vector4& color = Vector4::white
		);

		// Semi-Natural-Line Geometry
		void DrawRoundedLineTerminator(
			const Matrix3& model, const Vector2& point0, const Vector2& point1, float thickness = 0.5,
			const Vector4& color = Vector4::white
		);
		void DrawRoundedLineAnchor(
			const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& p2,
			float thickness = 0.5, const Vector4& color = Vector4::white
		);
		void DrawRoundedLine(
			const Matrix3& model, const Vector2& point0, const Vector2& point1, float thickness = 0.5,
			const Vector4& color = Vector4::white
		);

		// text
		void DrawText(
			const Matrix3& model, const std::string& text, const std::shared_ptr<Font>& font,
			const TextOptions& options = TextOptions()
		);
		void DrawTextGlyph(
			const Matrix3& model, const Vector2& point, const Vector2& size, const std::shared_ptr<Texture2D>& texture,
			const Vector2& uvPoint = Vector2::zero, const Vector2& uvSize = Vector2::one,
			const Vector4& color = Vector4::white
		);

	private:
		Shader geometryShader;
		Shader textShader;

		enum class DrawingMode
		{
			Geometry,
			Text,
		};

		DrawingMode drawingMode = DrawingMode::Geometry;

		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		std::vector<CanvasVertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<std::shared_ptr<Texture2D>> textures;
		std::vector<int> slots;

		std::shared_ptr<Texture2D> whiteTexture;

		size_t vertexOffset = 0;
		size_t indexOffset = 0;
		size_t textureOffset = 0;

		size_t drawCount = 0;

		size_t batchVertex = 512;
		size_t batchIndexMultiplier = 2;

		Matrix3 viewProjection;

	private:
		void InitializeVertexPool();
		void InitializeTexturePool();
		void initializeShaders();

		void SwitchDrawingMode(DrawingMode mode);
	};
}
