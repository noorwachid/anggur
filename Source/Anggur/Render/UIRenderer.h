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
	struct UIVertex
	{
		Vector2 position;
		Vector4 color;
		Vector2 uv;
		float slot;

		UIVertex() = default;
		UIVertex(const Vector2& newPosition, const Vector4& newColor, const Vector2& newUV, float newSlot = 0.0f)
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

	class UIRenderer
	{
	public:
		UIRenderer();

		~UIRenderer();

		void SetBatchChunk(size_t vertex, size_t indexMultiplier = 2);
		void Clear(const Vector4& color = Vector4::black);
		void SetViewport(const Vector2& size);
		void SetViewProjection(const Matrix3& newViewProjection);

		void BeginScene();
		void BeginScene(const Matrix3& viewProjection);
		void EndScene();

		bool IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize);
		void Flush();
		void FlushInternalBuffer();

		void Draw(
			const std::vector<UIVertex>& newVertices, 
			const std::vector<uint32_t>& newIndices,
			const std::shared_ptr<Texture2D>& newTexture
		);

		void DrawRectangle(const Vector2& position, const Vector2& size, const Vector4& color = Vector4::white);

		void DrawRectangle(const Vector2& position, const Vector2& size, const std::shared_ptr<Texture2D>& texture, const Vector2& texturePosition, const Vector2& textureSize, const Vector4& color = Vector4::white);

		Vector2 MeasureText(
			const std::string& text, 
			const std::shared_ptr<Font> font,
			float fontSize = 16.0f,
			float letterSpacing = 1.0f,
			float wordSpacing = 4.0f,
			float lineSpacing = 0.0f
		);

		void DrawText(
			const Vector2& position, 
			const Vector2& size, 
			const std::string& text, 
			const std::shared_ptr<Font> font,
			float fontSize = 16.0f,
			float letterSpacing = 1.0f,
			float wordSpacing = 4.0f,
			float lineSpacing = 0.0f
		);

		void DrawTextGlyph(
			const Vector2& point, 
			const Vector2& size, 
			const std::shared_ptr<Texture2D>& texture,
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

		std::vector<UIVertex> vertices;
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

