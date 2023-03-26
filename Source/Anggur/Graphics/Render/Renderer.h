#pragma once

#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Graphics/Render/Font.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include <memory>
#include <vector>

namespace Anggur
{
	struct Vertex
	{
		Vector2 position;
		Vector4 color;
		Vector2 uv;
		float slot;

		Vertex() = default;
		Vertex(const Vector2& newPosition, const Vector4& newColor, const Vector2& newUV, float newSlot = 0.0f)
			: position(newPosition), color(newColor), uv(newUV), slot(newSlot)
		{
		}
	};

	class Renderer
	{
	public:
		Renderer();

		~Renderer();

		void SetBatchChunk(usize vertex, usize indexMultiplier = 2);
		void Clear(const Vector4& color = Vector4::black);
		void SetViewport(const Vector2& position, const Vector2& size);
		void SetViewProjection(const Matrix3& newViewProjection);

		void BeginScissor(const Vector2& position, const Vector2& size);
		void EndScissor();

		void BeginScene();
		void BeginScene(const Matrix3& viewProjection);
		void EndScene();

		bool IsCapacityMaxout(usize newVertexSize, usize newIndexSize, usize newTextureSize);
		void Flush();
		void FlushInternalBuffer();

		void Draw(
			const std::vector<Vertex>& newVertices, 
			const std::vector<uint>& newIndices,
			Texture2D* texture
		);

		// Digital Geometry
		void DrawTriangle(
			const Matrix3& model, 
			const Vector2& position0, 
			const Vector2& position1, 
			const Vector2& position2,
			const Vector4& color = Vector4::white
		);
		void DrawQuad(
			const Matrix3& model, 
			const Vector2& position0, 
			const Vector2& position1, 
			const Vector2& position2,
			const Vector2& position3, 
			const Vector4& color = Vector4::white
		);

		void DrawRectangle(
			const Matrix3& model, 
			const Vector2& position, 
			const Vector2& size, 
			const Vector4& color = Vector4::white
		);

		void DrawRectangle(
			const Matrix3& model, 
			const Vector2& position, 
			const Vector2& size, 
			Texture2D* texture,
			const Vector2& texturePosition = Vector2::zero, 
			const Vector2& textureSize = Vector2::one,
			const Vector4& color = Vector4::white
		);

		// Augmented-Natural Geometry
		void DrawArc(
			const Matrix3& model, 
			float radius = 1.0f, 
			float beginAngle = 0.0f, 
			float sweepAngle = Math::pi,
			int segment = 16, 
			const Vector4& color = Vector4::white
		);
		void DrawCircle(
			const Matrix3& model, 
			float radius = 1.0f, 
			int segment = 32, 
			const Vector4& color = Vector4::white
		);

		// Line Geometry
		void DrawLineTerminator(
			const Matrix3& model, 
			const Vector2& position0, 
			const Vector2& position1, 
			float thickness = 0.5,
			const Vector4& color = Vector4::white
		);
		void DrawLineAnchor(
			const Matrix3& model, 
			const Vector2& position0, 
			const Vector2& position1, 
			const Vector2& p2,
			float thickness = 0.5, 
			const Vector4& color = Vector4::white
		);
		void DrawLine(
			const Matrix3& model, 
			const Vector2& position0, 
			const Vector2& position1, 
			float thickness = 0.5,
			const Vector4& color = Vector4::white
		);
		void DrawPolyLine(
			const Matrix3& model, 
			const std::vector<Vector2>& positions, 
			float thickness = 0.5,
			const Vector4& color = Vector4::white
		);
		void DrawClosedPolyLine(
			const Matrix3& model, 
			const std::vector<Vector2>& positions, 
			float thickness = 0.5,
			const Vector4& color = Vector4::white
		);

		// Text
		Vector2 MeasureText(
			const std::string& text, 
			Font* font, 
			float fontSize
		);

		void DrawText(
			const Matrix3& model, 
			const Vector2& position, 
			const std::string& text, 
			Font* font, 
			float fontSize,
			const Vector4& color = Vector4::white
		);

		void DrawTextGlyph(
			const Matrix3& model, 
			const Vector2& position, 
			const Vector2& size, 
			Texture2D* texture,
			const Vector2& texturePosition = Vector2::zero, 
			const Vector2& textureSize = Vector2::one,
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

		std::vector<Vertex> vertices;
		std::vector<uint> indices;
		std::vector<Texture2D*> textures;
		std::vector<int> slots;

		Texture2D* whiteTexture;

		usize vertexOffset = 0;
		usize indexOffset = 0;
		usize textureOffset = 0;

		usize drawCount = 0;

		usize batchVertex = 512;
		usize batchIndexMultiplier = 2;

		Matrix3 viewProjection;

	private:
		void InitializeVertexPool();
		void InitializeTexturePool();
		void InitializeShaders();

		void SwitchDrawingMode(DrawingMode mode);
	};
}
